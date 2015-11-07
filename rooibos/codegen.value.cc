#include "rooibos/codegen-internal.hh"

using std::make_shared;
using std::shared_ptr;

using llvm::Argument;
using llvm::ConstantInt;
using llvm::dyn_cast;
using llvm::Instruction;
using llvm::Type;
using llvm::Value;

namespace rooibos
{
  shared_ptr<ExpressionAST>
  coerce(Type * type, shared_ptr<ExpressionAST> expr)
  {
    return make_shared<BinaryExpressionAST>(expr, BinaryOp::BITWISE_OR,
        make_shared<NumberLiteralAST>(0));
  }

  shared_ptr<ExpressionAST>
  codegenDefaultValue(const Type * type)
  {
    return make_shared<NumberLiteralAST>(0);
  }

  shared_ptr<ExpressionAST>
  codegen(Identifiers & idents, Value * value)
  {
    shared_ptr<ExpressionAST> expr;
    if(!value)
    {
      return nullptr;
    }
      else if(Argument * arg = dyn_cast<Argument>(value))
    {
      expr = idents.forParameter(arg->getName());
    }
      else if(ConstantInt * k = dyn_cast<ConstantInt>(value))
    {
      expr = make_shared<NumberLiteralAST>(k->getSExtValue());
    }
      else if(Instruction * inst = dyn_cast<Instruction>(value))
    {
      expr = idents.forInstruction(*inst);
    }
      else
    {
      return nullptr;
    }
    return coerce(value->getType(), expr);
  }
}
