#include "rooibos/codegen.hh"

#include "rooibos/InstCodegenVisitor.hh"

using std::make_shared;
using std::set;
using std::shared_ptr;
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

      auto impl = make_shared<FunctionDeclarationAST>(funcIdent);

      vector<StatementAST::ptr> paramCoercions;
      for(auto & param : func.getArgumentList())
      {
        auto ident = idents.forParameter(param.getName());
        impl->params.push_back(ident);
        auto typeExpr = codegen(idents, &param);
        auto paramType = make_shared<AssignmentExpressionAST>(ident, typeExpr);
        paramCoercions.push_back(make_shared<ExpressionStatementAST>(paramType));
      }

      vector<VariableDeclaratorAST::ptr> vars;
      vector<StatementAST::ptr> stmts;
      InstCodegenVisitor instVisitor(ctx, vars, stmts);
      instVisitor.visit(func);

      if(ctx.needsStackPointer)
      {
        auto decl = make_shared<VariableDeclaratorAST>(ctx.idents.FP,
            make_shared<NumberLiteralAST>(0));
        vars.insert(vars.begin(), decl);

        auto coercedFP = make_shared<BinaryExpressionAST>(ctx.idents.SP,
            BinaryOp::BITWISE_OR, make_shared<NumberLiteralAST>(0));
        auto init = make_shared<AssignmentExpressionAST>(ctx.idents.FP,
            coercedFP);
        stmts.insert(stmts.begin(), make_shared<ExpressionStatementAST>(init));

        auto fpReset = make_shared<ExpressionStatementAST>(
            make_shared<AssignmentExpressionAST>(
              ctx.idents.SP,
              make_shared<BinaryExpressionAST>(
                ctx.idents.FP,
                BinaryOp::BITWISE_OR,
                make_shared<NumberLiteralAST>(0))));
        stmts.insert(stmts.end() - 1, fpReset);
      }

      if(!vars.empty())
      {
        auto decl = make_shared<VariableDeclarationAST>();
        decl->decls.insert(decl->decls.begin(), vars.begin(), vars.end());
        stmts.insert(stmts.begin(), decl);
      }

      auto & body = impl->body->body;
      body.insert(body.end(), paramCoercions.begin(), paramCoercions.end());
      body.insert(body.end(), stmts.begin(), stmts.end());

      impls.push_back(impl);
      asmRet->props.push_back(make_shared<PropertyAST>(funcIdent, funcIdent));
      adaptors->props.push_back(make_shared<PropertyAST>(
            externIdent,
            make_shared<MemberExpressionAST>(idents.asm_, funcIdent)));
  }
}
