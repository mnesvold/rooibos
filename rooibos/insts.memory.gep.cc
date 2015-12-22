#include "rooibos/InstCodegenVisitor.hh"

#include <llvm/IR/LLVMContext.h>

#include "rooibos/codegen.hh"
#include "rooibos/util.hh"

using std::make_shared;
using std::shared_ptr;

using llvm::CompositeType;
using llvm::ConstantInt;
using llvm::dyn_cast;
using llvm::GetElementPtrInst;
using llvm::SequentialType;
using llvm::StructType;
using llvm::Type;
using llvm::Value;

namespace rooibos
{
  namespace
  {
    const char * IMUL_FUNC_NAME = "Math__imul";

    ExpressionAST::ptr
    getTypeElementOffset(Identifiers & idents,
                         Type ** typePtr,
                         Value * index)
    {
      auto type = *typePtr;
      if(auto seqType = dyn_cast<SequentialType>(type))
      {
        auto elementType = seqType->getElementType();
        *typePtr = elementType;
        auto size = codegenTypeSize(elementType);
        auto step = codegen(idents, index);
        auto imul = idents.forStdlibFunc(IMUL_FUNC_NAME);
        auto call = CallExpressionAST::create(imul);
        call->arguments.push_back(NumberLiteralAST::create(size));
        call->arguments.push_back(step);
        return call;
      }
      if(auto structType = dyn_cast<StructType>(type))
      {
        auto k = dyn_cast<ConstantInt>(index);
        if(!k)
        {
          type->dump();
          index->dump();
          panic("^-- need a constant to subscript the above type");
        }
        auto offset = 0;
        for(unsigned int idx=0; idx<k->getValue().getZExtValue(); idx++)
        {
          offset += codegenTypeSize(structType->getElementType(idx));
        }
        return NumberLiteralAST::create(offset);
      }
      type->dump();
      panic("^-- is un-subscript-able type");
    }
  }

  void
  InstCodegenVisitor::visitGetElementPtrInst(GetElementPtrInst & inst)
  {
    _ctx.stdlib.insert(IMUL_FUNC_NAME);

    auto base = codegen(_ctx.idents, inst.getPointerOperand());

    auto type = inst.getPointerOperand()->getType();
    auto expr = base;
    for(auto it=inst.idx_begin(), end=inst.idx_end(); it != end; ++it)
    {
      auto offset = getTypeElementOffset(_ctx.idents, &type, it->get());
      expr = BinaryExpressionAST::create(expr, BinaryOp::ADD, offset);
    }
    _emit(inst, expr);
  }
}
