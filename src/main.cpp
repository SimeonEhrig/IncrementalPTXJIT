#include <cling/Interpreter/Interpreter.h>
#include <cling/Interpreter/Transaction.h>

#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>

#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <system_error>

#include "argc_helper.hpp"

int main(int argc, char **argv) {
  // create incremental compiler instance
  cling::Interpreter interp(argc, argv, LLVMDIR);

  // input -> input from the shell
  // buffer -> will be used to store block commands, like functions
  // output -> store all inputs (and later the compiled code), will be extended
  // continuesly
  std::string input, buffer;
  std::string prompt("> ");
  // will be used to format the input of block commands and store the
  // information, that the input is inside a block command
  unsigned int number_brackets = 0;
  // will be used, to format the output, without this value, the indent of the
  // start of a section will be wrong
  bool begin_section = false;

  Argc_helper argc_helper;

  switch (argc_helper.parse_args(argc, argv)) {
  case (Argc_helper::param::no_parameter):
    break;
  case (Argc_helper::param::help):
    argc_helper.print_help();
    return EXIT_SUCCESS;
  case (Argc_helper::param::load_file):
    // try to load source code from a file
    std::ifstream is(argv[1], std::ifstream::in);
    if (!is.is_open()) {
      std::cerr << "ERROR: cannot open file " << argv[1] << std::endl;
    } else {
      buffer = std::string(std::istreambuf_iterator<char>(is),
                           std::istreambuf_iterator<char>());
      interp.process(buffer);
      buffer = "";
      is.close();
    }
  }

  // main loop
  do {
    std::cout << prompt;
    getline(std::cin, input);

    // quit
    if (input == ".q")
      return EXIT_SUCCESS;
    // print llvm ir code of the last transaction
    else if (input == ".p")
      interp.getLastTransaction()->getModule()->dump();
    // execute command
    else if (input.find(".e", 0) == 0) {
      const std::string functionname = input.substr(3);
      interp.process(functionname);
    }
    // write source code to file
    else if (input.find(".f", 0) == 0) {
      const std::string path = input.substr(3);
      std::error_code EC;
      llvm::raw_fd_ostream os(path, EC, llvm::sys::fs::F_None);
      if (EC) {
        std::cerr << "ERROR: cannot generate file " << path << std::endl;
      } else {
        const cling::Transaction *T = interp.getFirstTransaction();
        // iterate over all transaction
        while (T->getNext() != nullptr) {
          T->getModule()->print(os, nullptr);
          T = T->getNext();
        }
        // the last transaction is a exception
        interp.getLastTransaction()->getModule()->print(os, nullptr);
        os.close();
      }
      // handle normal source
    } else {
      // if line is empty, add a empty line to the whole source code
      if (input == "") {
        // skip the empty line or add it to buffer, depending of the input
        // before
        if (number_brackets == 0) {
          continue;
        } else {
          buffer.append("\n");
        }
        continue;
      }

      // if the input ends with {, a block command starts
      if (input.rfind("{") == input.length() - 1) {
        begin_section = true;
        ++number_brackets;
      }

      // if the input ends with }, we leave a block
      if (input.rfind("}") == input.length() - 1) {
        --number_brackets;
        // if we leave all blocks, copy the buffer content to input
        // so, the compiler gets the whole block command as one input
        if (number_brackets == 0) {
          buffer.append(input + "\n");
          input = buffer;
          buffer = "";
        }
      }

      // if there is no block command, simply compile line by line
      if (number_brackets == 0) {
        prompt = "> ";
        interp.process(input, 0, 0, true);
      } else {
        // for getting a block command, the buffer will be used
        // the indent will calculated depending of the level of block command
        // (section)
        prompt = std::string(number_brackets * 3, ' ');
        if (begin_section) {
          // the beginning of a block (e.g a function head) is one level higher,
          // than the body
          buffer.append(std::string((number_brackets - 1) * 3, ' ') + input +
                        "\n");
          begin_section = false;
        } else {
          buffer.append(std::string(number_brackets * 3, ' ') + input + "\n");
        }
      }
    }
  } while (true);

  return EXIT_FAILURE;
}
