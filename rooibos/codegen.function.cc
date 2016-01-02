#include "rooibos/codegen.hh"

#include "rooibos/InstCodegenVisitor.hh"

using std::set;
using std::string;
using std::vector;

using llvm::Function;

namespace rooibos
{
  namespace
  {
    ExpressionStatementAST::ptr
    codegen_fp_prologue(CodegenContext & ctx)
    {
      auto coercedFP = BinaryExpressionAST::create(ctx.idents.SP,
          BinaryOp::BITWISE_OR, NumberLiteralAST::create(0));
      auto init = AssignmentExpressionAST::create(ctx.idents.FP, coercedFP);
      return ExpressionStatementAST::create(init);
    }

    ExpressionStatementAST::ptr
    codegen_fp_epilogue(CodegenContext & ctx)
    {
      return ExpressionStatementAST::create(
          AssignmentExpressionAST::create(
            ctx.idents.SP,
            BinaryExpressionAST::create(
              ctx.idents.FP,
              BinaryOp::BITWISE_OR,
              NumberLiteralAST::create(0))));
    }
  }

  void
  codegen(Function & func,
          CodegenContext & ctx,
          vector<StatementAST::ptr> & impls,
          ObjectExpressionAST::ptr asmRet,
          ObjectExpressionAST::ptr adaptors)
  {
      auto & idents = ctx.idents;

      if(func.isDeclaration())
      {
        return;
      }
      auto funcIdent = idents.forFunction(func.getName());
      auto externIdent = idents.forFunctionExtern(func.getName());

      auto impl = FunctionDeclarationAST::create(funcIdent);
      auto & body = impl->body->body;
      vector<StatementAST::ptr> paramCoercions, prologue, core, epilogue;
      vector<VariableDeclaratorAST::ptr> vars;

      for(auto & param : func.getArgumentList())
      {
        auto ident = idents.forParameter(param.getName());
        impl->params.push_back(ident);
        auto typeExpr = codegen(idents, &param);
        auto paramType = AssignmentExpressionAST::create(ident, typeExpr);
        paramCoercions.push_back(ExpressionStatementAST::create(paramType));
      }

      InstCodegenVisitor instVisitor(ctx, vars, core);
      instVisitor.visit(func);

      if(ctx.needsStackPointer)
      {
        auto decl = VariableDeclaratorAST::create(ctx.idents.FP,
            NumberLiteralAST::create(0));
        vars.insert(vars.begin(), decl);

        prologue.push_back(codegen_fp_prologue(ctx));
        epilogue.push_back(codegen_fp_epilogue(ctx));
      }

      body.insert(body.begin(), paramCoercions.begin(), paramCoercions.end());
      if(!vars.empty())
      {
        auto decl = VariableDeclarationAST::create();
        decl->decls.insert(decl->decls.begin(), vars.begin(), vars.end());
        body.push_back(decl);
      }
      body.insert(body.end(), prologue.begin(), prologue.end());
      body.insert(body.end(), core.begin(), core.end());
      body.insert(body.end(), epilogue.begin(), epilogue.end());

      impls.push_back(impl);
      asmRet->props.push_back(PropertyAST::create(funcIdent, funcIdent));
      adaptors->props.push_back(PropertyAST::create(
            externIdent,
            MemberExpressionAST::create(idents.asm_, funcIdent)));
  }
}
