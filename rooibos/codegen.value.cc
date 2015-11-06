#include "rooibos/codegen-internal.hh"

using std::make_shared;
using std::shared_ptr;

using llvm::Argument;
using llvm::ConstantInt;
using llvm::dyn_cast;
using llvm::Value;

namespace rooibos
{
  namespace
  {
    shared_ptr<ExpressionAST>
    coerce(shared_ptr<ExpressionAST> expr)
    {
      return make_shared<BinaryExpressionAST>(expr, BinaryOp::BITWISE_OR,
          make_shared<NumberLiteralAST>(0));
    }
  }

  shared_ptr<ExpressionAST>
  codegen(Identifiers & idents, Value * value)
  {
    if(!value)
    {
      return nullptr;
    }
    if(Argument * arg = dyn_cast<Argument>(value))
    {
      return coerce(idents.forParameter(arg->getName()));
    }
    if(ConstantInt * k = dyn_cast<ConstantInt>(value))
    {
      return coerce(make_shared<NumberLiteralAST>(k->getSExtValue()));
    }
    return nullptr;
  }
}
