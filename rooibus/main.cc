#include <iostream>
#include <memory>

#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/DataStream.h>
#include <llvm/Support/ErrorOr.h>
#include <llvm/Support/ManagedStatic.h>
#include <json.hpp>

#include "rooibus/ast.hh"
#include "rooibus/ast-json.hh"
#include "rooibus/codegen.hh"

int
main ( void )
{
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
  auto json = jsonifyAST(*ast);
  std::cout << std::setw(2) << *json << std::endl;

  return 0;
}
