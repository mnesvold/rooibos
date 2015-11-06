#include "rooibos/codegen-internal.hh"

using std::make_shared;
using std::shared_ptr;

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
      for(auto & param : func.getArgumentList())
      {
        auto ident = idents.forParameter(param.getName());
        impl->params.push_back(ident);
        auto typeExpr = codegen(idents, &param);
        auto paramType = make_shared<AssignmentExpressionAST>(ident, typeExpr);
        impl->body->body.push_back(
            make_shared<ExpressionStatementAST>(paramType));
      }

      InstCodegenVisitor instVisitor(idents, impl->body);
      instVisitor.visit(func);
      asmFunc->body->body.push_back(impl);

      asmRet->props.push_back(make_shared<PropertyAST>(funcIdent, funcIdent));

      adaptors->props.push_back(make_shared<PropertyAST>(
            externIdent,
            make_shared<MemberExpressionAST>(idents.asm_, funcIdent)));
  }
}
