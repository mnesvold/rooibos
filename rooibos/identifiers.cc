#include "rooibos/identifiers.hh"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>

#include <llvm/ADT/Twine.h>

using std::for_each;
using std::hex;
using std::isalnum;
using std::make_shared;
using std::ostringstream;
using std::setfill;
using std::setw;
using std::shared_ptr;
using std::string;

using llvm::Instruction;
using llvm::Twine;

namespace
{
  shared_ptr<rooibos::IdentifierAST>
  mkid(const string & name)
  {
    return make_shared<rooibos::IdentifierAST>(name);
  }

  string
  sanitize(const string & name)
  {
    ostringstream result;
    result << hex << setfill('0');
    for_each(name.begin(), name.end(), [&](string::value_type c)
    {
      if(c == '$')
        result << "$$";
      else if(c == '.')
        result << "$_";
      else if((c == '_') || isalnum(c))
        result << c;
      else
        result << '$' << setw(2) << (int)c;
    });
    return result.str();
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
    Float64Array(mkid("Float64Array")),
    FP(mkid("FP")),
    globals(mkid("globals")),
    heap(mkid("heap")),
    HEAP32(mkid("HEAP32")),
    HEAP64F(mkid("HEAP64F")),
    Int32Array(mkid("Int32Array")),
    SP(mkid("SP")),
    stdlib(mkid("stdlib")),
    this_(mkid("this"))
  {}

  IdentifierAST::ptr
  Identifiers::forFunction(const string & name)
  {
    return IdentifierAST::create("f_" + sanitize(name));
  }

  IdentifierAST::ptr
  Identifiers::forFunctionExtern(const string & name)
  {
    return IdentifierAST::create(sanitize(name));
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
    return IdentifierAST::create(ident);
  }

  IdentifierAST::ptr
  Identifiers::forParameter(const string & name)
  {
    return IdentifierAST::create("p_" + sanitize(name));
  }

  IdentifierAST::ptr
  Identifiers::forStdlibFunc(const string & name)
  {
    return IdentifierAST::create("stdlib_" + sanitize(name));
  }
}
