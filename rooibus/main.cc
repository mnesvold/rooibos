#include <iostream>
#include <memory>

#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/DataStream.h>
#include <llvm/Support/ErrorOr.h>
#include <llvm/Support/ManagedStatic.h>

#include "rooibus/ast.hh"

namespace rooibus
{

  template<class T, class... Args>
  std::unique_ptr<T> make_unique(Args&&... args)
  {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
  }

  std::unique_ptr<ProgramAST>
  codegen(llvm::Module & module)
  {
    auto program = make_unique<ProgramAST>();
    program->body.push_back(make_unique<EmptyStatementAST>());
    return program;
  }

}

int
main ( void )
{
  std::clog << "Rooibus Code Generator" << std::endl;

  llvm::LLVMContext & context = llvm::getGlobalContext();
  llvm::llvm_shutdown_obj shutdown;

  std::string errorMessage;

  std::unique_ptr<llvm::DataStreamer> streamer =
    llvm::getDataFileStreamer("-", &errorMessage);

  if(!streamer)
  {
    std::cerr << "Missing input data" << std::endl;
    return 1;
  }

  llvm::ErrorOr<std::unique_ptr<llvm::Module>> moduleOrErr =
    llvm::getStreamedBitcodeModule("<stdin>", std::move(streamer), context);

  if(!moduleOrErr)
  {
    std::cerr << "Bad input data" << std::endl;
    return 1;
  }

  std::unique_ptr<llvm::Module> module = move(*moduleOrErr);
  module->materializeAllPermanently();

  auto ast = rooibus::codegen(*module);
  std::cout << *ast << std::endl;

  return 0;
}
