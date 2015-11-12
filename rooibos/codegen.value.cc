#include "rooibos/codegen.hh"

#include <llvm/IR/Constants.h>

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
  ExpressionAST::ptr
  coerce(Type * type, ExpressionAST::ptr expr)
  {
    return make_shared<BinaryExpressionAST>(expr, BinaryOp::BITWISE_OR,
        make_shared<NumberLiteralAST>(0));
  }

  ExpressionAST::ptr
  codegenDefaultValue(const Type * type)
  {
    return make_shared<NumberLiteralAST>(0);
  }

  ExpressionAST::ptr
  codegen(Identifiers & idents, Value * value)
  {
    ExpressionAST::ptr expr;
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
