#include "rooibus/codegen.hh"

using std::forward;
using std::make_shared;
using std::move;
using std::unique_ptr;

using llvm::Module;

namespace
{
  template<class T, class... Args>
  unique_ptr<T> make_unique(Args&&... args)
  {
    return unique_ptr<T>(new T(forward<Args>(args)...));
  }

}

namespace rooibus
{
  unique_ptr<ProgramAST>
  codegen(Module & module)
  {
    auto program = make_unique<ProgramAST>();
    auto iifeFunc = make_shared<FunctionExpressionAST>();
    iifeFunc->params.push_back(make_shared<IdentifierAST>("globals"));
    auto iifeCall = make_shared<CallExpressionAST>(iifeFunc);
    iifeCall->arguments.push_back(make_shared<IdentifierAST>("this"));
    auto iifeCallStmt = make_shared<ExpressionStatementAST>(iifeCall);
    program->body.push_back(iifeCallStmt);
    return program;
  }
}
