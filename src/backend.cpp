#include "backend.hpp"

#include <llvm/ADT/SmallString.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

int backend::generate_ptx(std::shared_ptr<llvm::Module> module,
                          std::string &ptx) {
  llvm::SmallString<1024> ptx_code;
  llvm::raw_svector_ostream os(ptx_code);

  if (generate_ptx(module, os)) {
    llvm::errs() << "failed to execute generate_ptx() in "
                    "generate_ptx(llvm::Module, std::string ptx))";
    return 1;
  }

  ptx = ptx_code.str();
  return 0;
}

int backend::generate_ptx(std::shared_ptr<llvm::Module> module,
                          llvm::raw_pwrite_stream &os) {
  LLVMInitializeNVPTXTargetInfo();
  LLVMInitializeNVPTXTarget();
  LLVMInitializeNVPTXTargetMC();
  LLVMInitializeNVPTXAsmPrinter();

  std::string error;
  auto Target =
      llvm::TargetRegistry::lookupTarget(module->getTargetTriple(), error);

  if (!Target) {
    llvm::errs() << error;
    return 1;
  }

  // is not important, because PTX does not use any object format
  llvm::Optional<llvm::Reloc::Model> RM =
      llvm::Optional<llvm::Reloc::Model>(llvm::Reloc::Model::PIC_);

  llvm::TargetOptions TO = llvm::TargetOptions();

  // for a prototype the hard coded sm_20 is okay
  // in a productive application, the processor type should be used from the
  // arguments
  llvm::TargetMachine *targetMachine = Target->createTargetMachine(
      module->getTargetTriple(), "sm_20", "", TO, RM);
  module->setDataLayout(targetMachine->createDataLayout());

  llvm::SmallString<1024> ptx_code;
  llvm::raw_svector_ostream dest(ptx_code);

  llvm::legacy::PassManager pass;
  // it's important to use the type assembler
  // object file is not supported and do not make sense
  auto FileType = llvm::TargetMachine::CGFT_AssemblyFile;

  // addPassesToEmitFile() means compiling static code
  // addPassesToEmitMC() means compiling jitable code
  if (targetMachine->addPassesToEmitFile(pass, os, FileType)) {
    llvm::errs() << "TargetMachine can't emit assembler code";
    return 1;
  }

  pass.run(*module);
  return 0;
}
