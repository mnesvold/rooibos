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
  ReturnStatementAST::toJSON() const
  {
    return {
      { "type", "ReturnStatement" },
      { "argument", (argument ? argument->toJSON() : (json)nullptr) }
    };
  }

  json
  VariableDeclarationAST::toJSON() const
  {
    return {
      { "type", "VariableDeclaration" },
      { "declarations", jsonify(decls) },
      { "kind", "var" }
    };
  }

  json
  VariableDeclaratorAST::toJSON() const
  {
    return {
      { "type", "VariableDeclarator" },
      { "id", id->toJSON() },
      { "init", (init ? init->toJSON() : (json)nullptr) }
    };
  }

  json
  ObjectExpressionAST::toJSON() const
  {
    return {
      { "type", "ObjectExpression" },
      { "properties", jsonify(props) }
    };
  }

  json
  PropertyAST::toJSON() const
  {
    return {
      { "type", "Property" },
      { "key", key->toJSON() },
      { "value", value->toJSON() },
      { "kind", "init" }
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
        { "body", jsonify(body) }
      } },
      { "generator", false },
      { "expression", false }
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
  IdentifierAST::toJSON() const
  {
    return {
      { "type", "Identifier" },
      { "name", name }
    };
  }

  json
  LiteralAST::toJSON() const
  {
    return {
      { "type", "Literal" },
      { "value", value }
    };
  }
}
