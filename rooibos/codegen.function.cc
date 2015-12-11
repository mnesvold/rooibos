#include "rooibos/codegen.hh"

#include "rooibos/InstCodegenVisitor.hh"

using std::set;
using std::string;
using std::vector;

using llvm::Function;

namespace rooibos
{
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

      vector<StatementAST::ptr> paramCoercions;
      for(auto & param : func.getArgumentList())
      {
        auto ident = idents.forParameter(param.getName());
        impl->params.push_back(ident);
        auto typeExpr = codegen(idents, &param);
        auto paramType = AssignmentExpressionAST::create(ident, typeExpr);
        paramCoercions.push_back(ExpressionStatementAST::create(paramType));
      }

      vector<VariableDeclaratorAST::ptr> vars;
      vector<StatementAST::ptr> stmts;
      InstCodegenVisitor instVisitor(ctx, vars, stmts);
      instVisitor.visit(func);

      if(ctx.needsStackPointer)
      {
        auto decl = VariableDeclaratorAST::create(ctx.idents.FP,
            NumberLiteralAST::create(0));
        vars.insert(vars.begin(), decl);

        auto coercedFP = BinaryExpressionAST::create(ctx.idents.SP,
            BinaryOp::BITWISE_OR, NumberLiteralAST::create(0));
        auto init = AssignmentExpressionAST::create(ctx.idents.FP,
            coercedFP);
        stmts.insert(stmts.begin(), ExpressionStatementAST::create(init));

        auto fpReset = ExpressionStatementAST::create(
            AssignmentExpressionAST::create(
              ctx.idents.SP,
              BinaryExpressionAST::create(
                ctx.idents.FP,
                BinaryOp::BITWISE_OR,
                NumberLiteralAST::create(0))));
        stmts.insert(stmts.end() - 1, fpReset);
      }

      if(!vars.empty())
      {
        auto decl = VariableDeclarationAST::create();
        decl->decls.insert(decl->decls.begin(), vars.begin(), vars.end());
        stmts.insert(stmts.begin(), decl);
      }

      auto & body = impl->body->body;
      body.insert(body.end(), paramCoercions.begin(), paramCoercions.end());
      body.insert(body.end(), stmts.begin(), stmts.end());

      impls.push_back(impl);
      asmRet->props.push_back(PropertyAST::create(funcIdent, funcIdent));
      adaptors->props.push_back(PropertyAST::create(
            externIdent,
            MemberExpressionAST::create(idents.asm_, funcIdent)));
  }
}
