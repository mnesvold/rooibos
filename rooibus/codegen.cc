#include "rooibus/codegen-internal.hh"

using std::forward;
using std::make_shared;
using std::move;
using std::shared_ptr;
using std::string;
using std::unique_ptr;

using llvm::Module;

namespace {

  template<class T, class... Args>
  unique_ptr<T> make_unique(Args&&... args)
  {
    return unique_ptr<T>(new T(forward<Args>(args)...));
  }

}

namespace rooibus {

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
    asmFunc->body->body.push_back(make_shared<ExpressionStatementAST>(
          make_shared<StringLiteralAST>("use asm")));
    auto asmRetVal = make_shared<ObjectExpressionAST>();
    iifeFunc->body->body.push_back(make_shared<VariableDeclarationAST>(
         idents.ASM, asmFunc));

    auto asmCall = make_shared<CallExpressionAST>(idents.ASM);
    asmCall->arguments.push_back(idents.globals);
    iifeFunc->body->body.push_back(make_shared<VariableDeclarationAST>(
          idents.asm_, asmCall));

    auto adaptors = make_shared<ObjectExpressionAST>();
    iifeFunc->body->body.push_back(make_shared<VariableDeclarationAST>(
          idents.adaptors, adaptors));
    iifeFunc->body->body.push_back(make_shared<ExpressionStatementAST>(
          make_shared<AssignmentExpressionAST>(
            make_shared<MemberExpressionAST>(idents.globals, idents.asmExtern),
            idents.adaptors)));

    for(auto & func : module.getFunctionList())
    {
      codegen(func, idents, asmFunc, asmRetVal, adaptors);
    }
    asmFunc->body->body.push_back(make_shared<ReturnStatementAST>(asmRetVal));

    return program;
  }
}
