#ifndef ROOIBOS_AST_JSON_HH
#define ROOIBOS_AST_JSON_HH

#include "rooibos/ast.hh"

#include <memory>

#include <json.hpp>

namespace rooibos
{
  std::unique_ptr<nlohmann::json> jsonifyAST(ProgramAST &);
}

#endif
