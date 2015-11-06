#include "rooibos/codegen-internal.hh"

using std::make_shared;
using std::shared_ptr;
using std::string;

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
    ASM(mkid("ASM")),
    asm_(mkid("asm")),
    asmExtern(mkid("ASM")),
    ffi(mkid("ffi")),
    globals(mkid("globals")),
    heap(mkid("heap")),
    stdlib(mkid("stdlib")),
    this_(mkid("this"))
  {}

  shared_ptr<IdentifierAST>
  Identifiers::forFunction(const string & name)
  {
    return make_shared<IdentifierAST>("f_" + name);
  }

  shared_ptr<IdentifierAST>
  Identifiers::forFunctionExtern(const string & name)
  {
    return make_shared<IdentifierAST>(name);
  }

  shared_ptr<IdentifierAST>
  Identifiers::forParameter(const string & name)
  {
    return make_shared<IdentifierAST>("p_" + name);
  }
}