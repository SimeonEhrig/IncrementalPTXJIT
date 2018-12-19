#include "argc_helper.hpp"

Argc_helper::Argc_helper() {
  add_parameter("-h,--help", "print help");
  add_parameter("path/to/file", "path is optional and allows to preload code "
                                "from file and add some code at runtime");
  add_shell_command(".q", "leave the shell");
  add_shell_command(".e <command>", "execute command - c++ style: .e func();"
                                    " - at the moment just void function");
  add_shell_command(".p", "print llvm IR code of the last Inpute to shell");
  add_shell_command(".f <path/to/file>", "save compiled code in a file");
}

Argc_helper::param Argc_helper::parse_args(int argc, char **argv) {
  if (argc == 1)
    return Argc_helper::param::no_parameter;

  std::string arg1(argv[1]);

  if (arg1 == "-h" || arg1 == "--help")
    return Argc_helper::param::help;

  // at the moment, every parameter, which does not have a special meaning will
  // interpreted as path
  return Argc_helper::param::load_file;
}

void Argc_helper::add_parameter(std::string parameter,
                                std::string discription) {
  parameter_buffer.push_back(std::make_pair(parameter, discription));
  rebuild_help_text();
}

void Argc_helper::add_shell_command(std::string command,
                                    std::string discription) {
  command_buffer.push_back(std::make_pair(command, discription));
  rebuild_help_text();
}

void Argc_helper::rebuild_help_text() {
  help_text = "";

  help_text.append("command:\n");
  help_text.append("\tincrementalptxjit [ ");

  // beside, determine the longest parameter
  // it's necessary for the formatting of the parameter section
  // is integrated in this loop, to avoid a additional loop
  unsigned int max_parameter_lenght = 0;
  for (auto parameter : parameter_buffer) {
    help_text.append(parameter.first + " | ");
    max_parameter_lenght = (parameter.first.length() > max_parameter_lenght)
                               ? parameter.first.length()
                               : max_parameter_lenght;
  }
  // replace the last pipe symbol with a closed bracket
  help_text[help_text.length() - 2] = ']';
  help_text.append("\n\n");

  help_text.append("parameters:\n");
  for (auto parameter : parameter_buffer) {
    help_text.append("\t" + parameter.first);
    // add some space to arrange the description of all parameters on the same
    // column
    help_text.append(max_parameter_lenght - parameter.first.length(), ' ');
    help_text.append("   - " + parameter.second + "\n");
  }
  help_text.append("\n");

  help_text.append("shell commands:\n");
  unsigned int max_command_lenght = 0;
  for (auto commands : command_buffer) {
    max_command_lenght = (commands.first.length() > max_command_lenght)
                             ? commands.first.length()
                             : max_command_lenght;
  }

  for (auto commands : command_buffer) {
    help_text.append("\t" + commands.first);
    help_text.append(max_command_lenght - commands.first.length(), ' ');
    help_text.append("   - " + commands.second + "\n");
  }

  help_text.append("\n");
}

void Argc_helper::print_help(std::ostream &output /* = std::cout */) {
  output << help_text;
}
