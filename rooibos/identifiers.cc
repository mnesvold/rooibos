#include "rooibos/identifiers.hh"

#include <llvm/ADT/Twine.h>

using std::make_shared;
using std::shared_ptr;
using std::string;

using llvm::Instruction;
using llvm::Twine;

namespace
{
  shared_ptr<rooibos::IdentifierAST>
  mkid(const std::string & name)
  {
    return make_shared<rooibos::IdentifierAST>(name);
  }
}

namespace rooibos
{
  Identifiers::Identifiers()
  : adaptors(mkid("adaptors")),
    ArrayBuffer(mkid("ArrayBuffer")),
    ASM(mkid("ASM")),
    asm_(mkid("asm")),
    asmExtern(mkid("ASM")),
    ffi(mkid("ffi")),
    FP(mkid("FP")),
    globals(mkid("globals")),
    heap(mkid("heap")),
    HEAP32(mkid("HEAP32")),
    Int32Array(mkid("Int32Array")),
    SP(mkid("SP")),
    stdlib(mkid("stdlib")),
    this_(mkid("this"))
  {}

  IdentifierAST::ptr
  Identifiers::forFunction(const string & name)
  {
    return make_shared<IdentifierAST>("f_" + name);
  }

  IdentifierAST::ptr
  Identifiers::forFunctionExtern(const string & name)
  {
    return make_shared<IdentifierAST>(name);
  }

  IdentifierAST::ptr
  Identifiers::forInstruction(const Instruction & inst)
  {
    unsigned int id;
    if(_instIDMap.count(&inst))
    {
      id = _instIDMap[&inst];
    }
      else
    {
      id = _nextInstID++;
      _instIDMap[&inst] = id;
    }
    auto ident = ("l_" + Twine(id)).str();
    return make_shared<IdentifierAST>(ident);
  }

  IdentifierAST::ptr
  Identifiers::forParameter(const string & name)
  {
    return make_shared<IdentifierAST>("p_" + name);
  }

  IdentifierAST::ptr
  Identifiers::forStdlibFunc(const string & name)
  {
    return make_shared<IdentifierAST>("stdlib_" + name);
  }
}
