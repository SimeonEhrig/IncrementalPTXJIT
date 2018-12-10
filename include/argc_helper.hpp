#ifndef ARGC_HELPER_H
#define ARGC_HELPER_H

#include <iostream>
#include <string>
#include <utility>
#include <vector>

class Argc_helper {
private:
  // the buffers stores pairs of keywords and description
  // the class generate automatically a well formatted help text from the
  // vectors parameter_buffer stores all start up parameters of the application
  std::vector<std::pair<std::string, std::string>> parameter_buffer;
  // command_buffer stores all commands, which can be used during runtime
  std::vector<std::pair<std::string, std::string>> command_buffer;
  std::string help_text;

  /**
   * @brief generate and format help text from parameter_buffer and
   * command_buffer
   *
   */
  void rebuild_help_text();

public:
  enum param { no_parameter, help, load_file };

  Argc_helper();

  /**
   * @brief parse args of main and search after keywords
   *
   * @param argc number of arguments
   * @param argv list of arguments
   * @return Argc_helper::param
   */
  Argc_helper::param parse_args(int argc, char **argv);
  /**
   * @brief Add a new parameter with description. The help_text will be updated
   * automatically. parse_args() have to be updated manual.
   *
   * @param parameter parameter, for example -h
   * @param discription description of the parameter
   */
  void add_parameter(std::string parameter, std::string discription);
  /**
   * @brief Add a new command with description. The help_text will be updated
   * automatically.
   *
   * @param command command, for example .q
   * @param discription description of the command
   */
  void add_shell_command(std::string command, std::string discription);
  /**
   * @brief print the help text
   *
   * @param output Choose a output channel. By default, std::cout will be used.
   */
  void print_help(std::ostream &output = std::cout);
};

#endif // ARGC_HELPER_H
