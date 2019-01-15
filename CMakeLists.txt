cmake_minimum_required(VERSION 3.5)

# add cling dependencies
# https://github.com/root-project/cling/tree/master/tools/demo

set(LLVM_NO_DEAD_STRIP 1)

project(incrementalptxjit)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(incrementalptxjit)

find_package(cling REQUIRED)

include_directories(include/)

add_executable(incrementalptxjit
               src/main.cpp
               src/argc_helper.cpp
               src/backend.cpp)

target_link_libraries(incrementalptxjit
                      clingInterpreter
                      LLVMSupport
                      LLVMCore
                      LLVMNVPTXAsmPrinter
                      LLVMNVPTXCodeGen
                      LLVMNVPTXDesc
                      LLVMNVPTXInfo
                      #LLVMTarget
                      LLVMMC
                      )

target_compile_options(incrementalptxjit PUBLIC
                       -DLLVMDIR="${LLVM_INSTALL_PREFIX}"
                       -I${LLVM_INSTALL_PREFIX}/include)

set_target_properties(incrementalptxjit
    PROPERTIES ENABLE_EXPORTS 1)

install(TARGETS incrementalptxjit RUNTIME DESTINATION bin)
