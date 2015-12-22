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
using llvm::StructType;
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

  long
  codegenTypeSize(const llvm::Type * type)
  {
    if(auto bitWidth = type->getPrimitiveSizeInBits())
    {
      return (bitWidth + 7) / 8;
    }
    if(auto structType = dyn_cast<StructType>(type))
    {
      if(structType->isPacked())
      {
        type->dump();
        panic("^-- is un-size-able type (because it is packed)");
      }
      auto total = 0;
      for(auto it=structType->element_begin(), end=structType->element_end();
          it != end;
          ++it)
      {
        total += codegenTypeSize(*it);
      }
      return total;
    }
    type->dump();
    panic("^-- is un-size-able type");
  }

  IdentifierAST::ptr
  codegenHeapIdent(Identifiers & idents, const Type * type)
  {
    if(type->isIntegerTy())
    {
      switch(type->getPrimitiveSizeInBits())
      {
        case 32:
          return idents.HEAP32;
        default:
          type->dump();
          panic("^-- is un-heap-able type (because of its bit width)");
      }
    }
    if(type->isDoubleTy())
    {
      return idents.HEAP64F;
    }
    type->dump();
    panic("^-- is un-heap-able type");
  }

  unsigned
  codegenHeapShift(const llvm::Type * type)
  {
    if(type->isIntegerTy())
    {
      switch(type->getPrimitiveSizeInBits())
      {
        case 32:
          return 2;
        default:
          type->dump();
          panic("^-- is un-heap-shift-able type (because of its bit width)");
      }
    }
    if(type->isDoubleTy())
    {
      return 3;
    }
    type->dump();
    panic("^-- is un-heap-shift-able type");
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
