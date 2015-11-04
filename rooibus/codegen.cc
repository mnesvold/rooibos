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

    auto asmFunc = make_shared<FunctionExpressionAST>();
    asmFunc->params.push_back(make_shared<IdentifierAST>("stdlib"));
    asmFunc->params.push_back(make_shared<IdentifierAST>("ffi"));
    asmFunc->params.push_back(make_shared<IdentifierAST>("heap"));
    iifeFunc->body.push_back(make_shared<VariableDeclarationAST>(
          make_shared<IdentifierAST>("ASM"), asmFunc));

    return program;
  }
}
