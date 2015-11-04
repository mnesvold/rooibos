#include "rooibus/codegen.hh"

using std::forward;
using std::make_shared;
using std::move;
using std::shared_ptr;
using std::unique_ptr;

using llvm::Module;

using rooibus::IdentifierAST;

namespace
{
  template<class T, class... Args>
  unique_ptr<T> make_unique(Args&&... args)
  {
    return unique_ptr<T>(new T(forward<Args>(args)...));
  }

  struct Identifiers
  {
    const shared_ptr<IdentifierAST>
      adaptors = make_shared<IdentifierAST>("adaptors"),
      ASM = make_shared<IdentifierAST>("ASM"),
      asm_ = make_shared<IdentifierAST>("asm"),
      ffi = make_shared<IdentifierAST>("ffi"),
      globals = make_shared<IdentifierAST>("globals"),
      heap = make_shared<IdentifierAST>("heap"),
      stdlib = make_shared<IdentifierAST>("stdlib"),
      this_ = make_shared<IdentifierAST>("this");
  };

}

namespace rooibus
{
  unique_ptr<ProgramAST>
  codegen(Module & module)
  {
    Identifiers idents;

    auto program = make_unique<ProgramAST>();
    auto iifeFunc = make_shared<FunctionExpressionAST>();
    iifeFunc->params.push_back(idents.globals);
    auto iifeCall = make_shared<CallExpressionAST>(iifeFunc);
    iifeCall->arguments.push_back(idents.this_);
    auto iifeCallStmt = make_shared<ExpressionStatementAST>(iifeCall);
    program->body.push_back(iifeCallStmt);

    auto asmFunc = make_shared<FunctionExpressionAST>();
    asmFunc->params.push_back(idents.stdlib);
    asmFunc->params.push_back(idents.ffi);
    asmFunc->params.push_back(idents.heap);
    asmFunc->body.push_back(make_shared<ExpressionStatementAST>(
          make_shared<LiteralAST>("use asm")));
    asmFunc->body.push_back(make_shared<ReturnStatementAST>(
          make_shared<ObjectExpressionAST>()));
    iifeFunc->body.push_back(make_shared<VariableDeclarationAST>(
         idents.ASM, asmFunc));

    auto asmCall = make_shared<CallExpressionAST>(idents.ASM);
    asmCall->arguments.push_back(idents.globals);
    iifeFunc->body.push_back(make_shared<VariableDeclarationAST>(
          idents.asm_, asmCall));

    auto adaptors = make_shared<ObjectExpressionAST>();
    iifeFunc->body.push_back(make_shared<VariableDeclarationAST>(
          idents.adaptors, adaptors));
    iifeFunc->body.push_back(make_shared<ReturnStatementAST>(idents.adaptors));

    return program;
  }
}
