#include "rooibus/codegen-internal.hh"

using std::make_shared;
using std::shared_ptr;

using llvm::Function;

namespace rooibus
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
      InstCodegenVisitor instVisitor(idents, impl->body);
      instVisitor.visit(func);
      asmFunc->body->body.push_back(impl);

      asmRet->props.push_back(make_shared<PropertyAST>(funcIdent, funcIdent));

      adaptors->props.push_back(make_shared<PropertyAST>(
            externIdent,
            make_shared<MemberExpressionAST>(idents.asm_, funcIdent)));
  }
}
