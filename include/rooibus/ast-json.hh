#include "rooibus/ast.hh"

#include <memory>

#include <json.hpp>

namespace rooibus
{
  std::unique_ptr<nlohmann::json> jsonifyAST(ProgramAST &);
}
