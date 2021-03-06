#include "rooibos/ast-json.hh"

using std::enable_if;
using std::is_convertible;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::vector;

using nlohmann::json;

namespace rooibos
{
  namespace
  {
    class ExpressionJSONifier : public PatternVisitor
    {
    public:
      ExpressionJSONifier(json & j) : j(j) {}

      void visit(const DoubleLiteralAST & ast) override;
      void visit(const NumberLiteralAST & ast) override;
      void visit(const StringLiteralAST & ast) override;
      void visit(const AssignmentExpressionAST & ast) override;
      void visit(const BinaryExpressionAST &) override;
      void visit(const CallExpressionAST & ast) override;
      void visit(const ConditionalExpressionAST & ast) override;
      void visit(const FunctionExpressionAST & ast) override;
      void visit(const IdentifierAST & ast) override;
      void visit(const MemberExpressionAST & ast) override;
      void visit(const NewExpressionAST & ast) override;
      void visit(const ObjectExpressionAST & ast) override;
      void visit(const SubscriptExpressionAST & ast) override;
      void visit(const UnaryExpressionAST & ast) override;

    private:
      json & j;
    };

    class StatementJSONifier : public StatementVisitor
    {
    public:
      StatementJSONifier(json & j) : j(j) {}

      void visit(const FunctionDeclarationAST & ast) override;
      void visit(const VariableDeclarationAST & ast) override;
      void visit(const BlockStatementAST & ast) override;
      void visit(const ContinueStatementAST & ast) override;
      void visit(const EmptyStatementAST & ast) override;
      void visit(const ExpressionStatementAST & ast) override;
      void visit(const ReturnStatementAST & ast) override;
      void visit(const SwitchStatementAST & ast) override;
      void visit(const WhileStatementAST & ast) override;

    private:
      json & j;
    };

    template<typename E, typename V>
    void _recurse(json & j, const string & key, shared_ptr<E> ast)
    {
      if(!ast)
      {
        j[key] = nullptr;
        return;
      }
      V visitor(j[key]);
      ast->accept(visitor);
    }

    template<typename E>
    typename enable_if<is_convertible<E &, StatementAST &>::value>::type
    recurse(json & j, const string & key, shared_ptr<E> ast)
    {
      _recurse<StatementAST, StatementJSONifier>(j, key, ast);
    }

    template<typename E>
    typename enable_if<is_convertible<E &, PatternAST &>::value>::type
    recurse(json & j, const string & key, shared_ptr<E> ast)
    {
      _recurse<PatternAST, ExpressionJSONifier>(j, key, ast);
    }

    template<typename E,typename V>
    void
    _recurse(json & j, const string & key, const vector<shared_ptr<E>> & ast)
    {
      json array = json::array();
      for(auto & e : ast)
      {
        json element;
        V visitor(element);
        e->accept(visitor);
        array += element;
      }
      j[key] = array;
    }

    template<typename E>
    typename enable_if<is_convertible<E &,PatternAST &>::value>::type
    recurse(json & j, const string & key, const vector<shared_ptr<E>> & ast)
    {
      _recurse<E,ExpressionJSONifier>(j, key, ast);
    }

    template<typename E>
    typename enable_if<is_convertible<E &,StatementAST &>::value>::type
    recurse(json & j, const string & key, const vector<shared_ptr<E>> & ast)
    {
      _recurse<E,StatementJSONifier>(j, key, ast);
    }

    void ExpressionJSONifier::visit(const DoubleLiteralAST & ast)
    {
      j = {
        { "type", "Literal" },
        { "value", ast.value }
      };
    }

    void ExpressionJSONifier::visit(const NumberLiteralAST & ast)
    {
      j = {
        { "type", "Literal" },
        { "value", ast.value }
      };
    }

    void ExpressionJSONifier::visit(const StringLiteralAST & ast)
    {
      j = {
        { "type", "Literal" },
        { "value", ast.value }
      };
    }

    void ExpressionJSONifier::visit(const BinaryExpressionAST & ast)
    {
      j = {
        { "type", "BinaryExpression" },
        { "operator", ast.op }
      };
      recurse(j, "left", ast.lhs);
      recurse(j, "right", ast.rhs);
    }

    void ExpressionJSONifier::visit(const AssignmentExpressionAST & ast)
    {
      j = {
        { "type", "AssignmentExpression" },
        { "operator", "=" },
      };
      recurse(j, "left", ast.lhs);
      recurse(j, "right", ast.rhs);
    }

    void ExpressionJSONifier::visit(const CallExpressionAST & ast)
    {
      j = {
        { "type", "CallExpression" },
      };
      recurse(j, "callee", ast.callee);
      recurse(j, "arguments", ast.arguments);
    }

    void ExpressionJSONifier::visit(const ConditionalExpressionAST & ast)
    {
      j = {
        { "type", "ConditionalExpression" }
      };
      recurse(j, "test", ast.test);
      recurse(j, "consequent", ast.consequent);
      recurse(j, "alternate", ast.alternate);
    }

    void ExpressionJSONifier::visit(const FunctionExpressionAST & ast)
    {
      j = {
        { "type", "FunctionExpression" },
        { "id", nullptr },
        { "defaults", json::array() },
        { "rest", nullptr },
        { "generator", false },
        { "expression", false }
      };
      recurse(j, "params", ast.params);
      recurse(j, "body", ast.body);
    }

    void ExpressionJSONifier::visit(const IdentifierAST & ast)
    {
      j = {
        { "type", "Identifier" },
        { "name", ast.name }
      };
    }

    void ExpressionJSONifier::visit(const MemberExpressionAST & ast)
    {
      j = {
        { "type", "MemberExpression" },
        { "computed", false }
      };
      recurse(j, "object", ast.object);
      recurse(j, "property", ast.property);
    }

    void ExpressionJSONifier::visit(const NewExpressionAST & ast)
    {
      j = {
        { "type", "NewExpression" },
      };
      recurse(j, "callee", ast.callee);
      recurse(j, "arguments", ast.arguments);
    }

    void ExpressionJSONifier::visit(const ObjectExpressionAST & ast)
    {
      j = {
        { "type", "ObjectExpression" },
        { "properties", json::array() }
      };
      for(auto & prop : ast.props)
      {
        json propJson = {
          { "type", "Property" },
          { "kind", "init" }
        };
        recurse(propJson, "key", prop->key);
        recurse(propJson, "value", prop->value);
        j["properties"] += propJson;
      }
    }

    void ExpressionJSONifier::visit(const SubscriptExpressionAST & ast)
    {
      j = {
        { "type", "MemberExpression" },
        { "computed", true }
      };
      recurse(j, "object", ast.object);
      recurse(j, "property", ast.subscript);
    }

    void ExpressionJSONifier::visit(const UnaryExpressionAST & ast)
    {
      j = {
        { "type", "UnaryExpression" },
        { "prefix", true },
        { "operator", ast.op }
      };
      recurse(j, "argument", ast.argument);
    }

    void StatementJSONifier::visit(const FunctionDeclarationAST & ast)
    {
      j = {
        { "type", "FunctionDeclaration" },
        { "defaults", json::array() },
        { "rest", nullptr },
        { "generator", false },
        { "expression", false }
      };
      recurse(j, "id", ast.name);
      recurse(j, "params", ast.params);
      recurse(j, "body", ast.body);
    }

    void StatementJSONifier::visit(const VariableDeclarationAST & ast)
    {
      j = {
        { "type", "VariableDeclaration" },
        { "kind", "var" }
      };
      for(auto & decl : ast.decls)
      {
        json declJson = {
          { "type", "VariableDeclarator" },
        };
        recurse(declJson, "id", decl->id);
        recurse(declJson, "init", decl->init);
        j["declarations"] += declJson;
      }
    }

    void StatementJSONifier::visit(const BlockStatementAST & ast)
    {
      j = {
        { "type", "BlockStatement" }
      };
      recurse(j, "body", ast.body);
    }

    void StatementJSONifier::visit(const ContinueStatementAST & ast)
    {
      j = {
        { "type", "ContinueStatement" },
        { "label", nullptr }
      };
    }

    void StatementJSONifier::visit(const EmptyStatementAST & ast)
    {
      j = {
        { "type", "EmptyStatement" }
      };
    }

    void StatementJSONifier::visit(const ExpressionStatementAST & ast)
    {
      j = {
        { "type", "ExpressionStatement" }
      };
      recurse(j, "expression", ast.expression);
    }

    void StatementJSONifier::visit(const ReturnStatementAST & ast)
    {
      j = {
        { "type", "ReturnStatement" }
      };
      recurse(j, "argument", ast.argument);
    }

    void StatementJSONifier::visit(const SwitchStatementAST & ast)
    {
      j = {
        { "type", "SwitchStatement" },
        { "cases", json::array() },
        { "lexical", false }
      };
      recurse(j, "discriminant", ast.discriminant);
      for(auto & caseAST : ast.cases)
      {
        json caseJSON = {
          { "type", "SwitchCase" },
        };
        recurse(caseJSON, "test", caseAST->test);
        recurse(caseJSON, "consequent", caseAST->consequent);
        j["cases"] += caseJSON;
      }
    }

    void StatementJSONifier::visit(const WhileStatementAST & ast)
    {
      j = {
        { "type", "WhileStatement" }
      };
      recurse(j, "test", ast.test);
      recurse(j, "body", ast.body);
    }
  }

  unique_ptr<json>
  jsonifyAST(ProgramAST & program)
  {
    unique_ptr<json> ptr(new json {
      { "type", "Program" }
    });
    recurse(*ptr, "body", program.body);
    return ptr;
  }
}
