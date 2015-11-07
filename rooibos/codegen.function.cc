#include "rooibos/codegen-internal.hh"

using std::make_shared;
using std::shared_ptr;
using std::vector;

using llvm::Function;

namespace rooibos
{
  void
  codegen(Function & func,
          Identifiers & idents,
          shared_ptr<FunctionExpressionAST> asmFunc,
          shared_ptr<ObjectExpressionAST> asmRet,
          shared_ptr<ObjectExpressionAST> adaptors)
  {
      auto funcIdent = idents.forFunction(func.getName());
      auto externIdent = idents.forFunctionExtern(func.getName());

      auto impl = make_shared<FunctionDeclarationAST>(funcIdent);

      vector<shared_ptr<StatementAST>> paramCoercions;
      for(auto & param : func.getArgumentList())
      {
        auto ident = idents.forParameter(param.getName());
        impl->params.push_back(ident);
        auto typeExpr = codegen(idents, &param);
        auto paramType = make_shared<AssignmentExpressionAST>(ident, typeExpr);
        paramCoercions.push_back(make_shared<ExpressionStatementAST>(paramType));
      }

      vector<shared_ptr<StatementAST>> stmts;
      InstCodegenVisitor instVisitor(idents, stmts);
      instVisitor.visit(func);

      auto & body = impl->body->body;
      body.insert(body.end(), paramCoercions.begin(), paramCoercions.end());
      body.insert(body.end(), stmts.begin(), stmts.end());

      asmFunc->body->body.push_back(impl);
      asmRet->props.push_back(make_shared<PropertyAST>(funcIdent, funcIdent));

      adaptors->props.push_back(make_shared<PropertyAST>(
            externIdent,
            make_shared<MemberExpressionAST>(idents.asm_, funcIdent)));
  }
}
