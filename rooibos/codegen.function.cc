#include "rooibos/codegen.hh"

#include "rooibos/InstCodegenVisitor.hh"
#include "rooibos/InstContextVisitor.hh"

using std::set;
using std::string;
using std::vector;

using llvm::Function;

namespace rooibos
{
  namespace
  {
    VariableDeclaratorAST::ptr
    codegen_fp_var_decl(CodegenContext & ctx)
    {
      return VariableDeclaratorAST::create(ctx.idents.FP,
          NumberLiteralAST::create(0));
    }

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

      InstContextVisitor reqs;
      reqs.visit(func);

      auto impl = FunctionDeclarationAST::create(funcIdent);
      auto & body = impl->body->body;
      vector<StatementAST::ptr> paramCoercions, prologue, core, epilogue;
      vector<VariableDeclaratorAST::ptr> vars;

      if(reqs.needsFramePointer())
      {
        ctx.needsStackPointer = true;
        vars.insert(vars.begin(), codegen_fp_var_decl(ctx));
        prologue.push_back(codegen_fp_prologue(ctx));
        epilogue.push_back(codegen_fp_epilogue(ctx));
      }

      for(auto & param : func.getArgumentList())
      {
        auto ident = idents.forParameter(param.getName());
        impl->params.push_back(ident);
        auto typeExpr = codegen(idents, &param);
        auto paramType = AssignmentExpressionAST::create(ident, typeExpr);
        paramCoercions.push_back(ExpressionStatementAST::create(paramType));
      }

      InstCodegenVisitor instVisitor(ctx, vars, core, epilogue);
      instVisitor.visit(func);

      body.insert(body.begin(), paramCoercions.begin(), paramCoercions.end());
      if(!vars.empty())
      {
        auto decl = VariableDeclarationAST::create();
        decl->decls.insert(decl->decls.begin(), vars.begin(), vars.end());
        body.push_back(decl);
      }
      body.insert(body.end(), prologue.begin(), prologue.end());
      body.insert(body.end(), core.begin(), core.end());

      impls.push_back(impl);
      asmRet->props.push_back(PropertyAST::create(funcIdent, funcIdent));
      adaptors->props.push_back(PropertyAST::create(
            externIdent,
            MemberExpressionAST::create(idents.asm_, funcIdent)));
  }
}
