#include "rooibos/codegen.hh"

#include "rooibos/util.hh"

#include <llvm/IR/Constants.h>

using std::make_shared;
using std::shared_ptr;

using llvm::Argument;
using llvm::ConstantFP;
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
    if(type->isIntegerTy() || type->isPointerTy())
    {
      return BinaryExpressionAST::create(expr, BinaryOp::BITWISE_OR,
          NumberLiteralAST::create(0));
    }
      else if(type->isDoubleTy())
    {
      return UnaryExpressionAST::create(UnaryOp::PLUS, expr);
    }
      else
    {
      type->dump();
      panic("^-- is un-codegen-able type");
    }
  }

  ExpressionAST::ptr
  codegenDefaultValue(const Type * type)
  {
    if(type->isIntegerTy() || type->isPointerTy())
    {
      return NumberLiteralAST::create(0);
    }
      else if(type->isDoubleTy())
    {
      return DoubleLiteralAST::create(0.1);
    }
      else
    {
      type->dump();
      panic("^-- is un-codegen-able type");
    }
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
      else if(ConstantFP * k = dyn_cast<ConstantFP>(value))
    {
      expr = NumberLiteralAST::create(k->getValueAPF().convertToDouble());
    }
      else if(ConstantInt * k = dyn_cast<ConstantInt>(value))
    {
      expr = NumberLiteralAST::create(k->getSExtValue());
    }
      else if(Instruction * inst = dyn_cast<Instruction>(value))
    {
      expr = idents.forInstruction(*inst);
    }
      else
    {
      value->dump();
      panic("^-- has un-codegen-able type");
    }
    return coerce(value->getType(), expr);
  }
}
