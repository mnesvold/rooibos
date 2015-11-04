#include "rooibus/ast.hh"

using std::shared_ptr;
using nlohmann::json;

namespace
{
  template<typename C>
  json
  jsonify(const C & container)
  {
    json j = json::array();
    for(auto & el : container)
    {
      j += el->toJSON();
    }
    return j;
  }
}

namespace rooibus
{
  json
  ProgramAST::toJSON() const
  {
    json j;
    j["type"] = "Program";
    j["body"] = {};
    for(auto & stmt : body)
    {
      j["body"] += stmt->toJSON();
    }
    return j;
  }

  json
  EmptyStatementAST::toJSON() const
  {
    return { { "type", "EmptyStatement" } };
  }

  json
  ExpressionStatementAST::toJSON() const
  {
    return {
      { "type", "ExpressionStatement" },
      { "expression", expression->toJSON() }
    };
  }

  json
  CallExpressionAST::toJSON() const
  {
    return {
      { "type", "CallExpression" },
      { "callee", callee->toJSON() },
      { "arguments", jsonify(arguments) }
    };
  }

  json
  FunctionExpressionAST::toJSON() const
  {
    return {
      { "type", "FunctionExpression" },
      { "id", nullptr },
      { "params", jsonify(params) },
      { "defaults", json::array() },
      { "rest", nullptr },
      { "body", {
        { "type", "BlockStatement" },
        { "body", json::array() }
      } },
      { "generator", false },
      { "expression", false }
    };
  }

  json
  IdentifierAST::toJSON() const
  {
    return {
      { "type", "Identifier" },
      { "name", name }
    };
  }
}
