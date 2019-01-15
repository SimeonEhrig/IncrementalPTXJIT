#ifndef BACKEND_H
#define BACKEND_H

#include <llvm/IR/Module.h>
#include <llvm/Support/raw_ostream.h>

#include <string>

/**
 * @brief This Class provides functions to compile llvm ir code to ptx
 *
 */
namespace backend {

/**
 * @brief Compiles llvm::Module to PTX code an store the result in a string
 *
 * @param module llvm::Module with llvm ir
 * @param ptx contains the PTX code
 * @return 0 if successful otherwise 1
 */
int generate_ptx(std::shared_ptr<llvm::Module> module, std::string &ptx);

/**
 * @brief Compiles llvm::Module to PTX code and write it os
 *
 * @param module llvm::Module with llvm ir
 * @param ptx output stream where PTX code is writing in
 * @return 0 if successful otherwise 1
 */
int generate_ptx(std::shared_ptr<llvm::Module> module,
                 llvm::raw_pwrite_stream &os);

} // namespace backend

#endif // BACKEND_H
