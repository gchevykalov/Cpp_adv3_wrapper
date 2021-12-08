#pragma once

#include "wrapper.hpp"

/**
 * @brief Engine class
 * Stores command wrappers and execute them by names and arguments' values
 */
class engine_t {
private:
  std::map<string, wrapper_t*> commands;  ///< map of pointers on wrappers with given name
public:
  /**
   * Register new wrapper
   * @param[in] command - wrapper which stores the method
   * @param[in] commandName - the name that will be used for the appeal
   * @warning Method can throw exceptions
   */
  void registerCommand(wrapper_t* command, string const& commandName) {
    if (command == nullptr)
      throw exception("Unexpected 'nullptr'");
  
    if (commands.find(commandName) != commands.end())
      throw exception("Command with the same name already exists");
  
    commands[commandName] = command;
  }
  
  /**
   * Execute command with specified name
   * @param[in] commandName - name of command that will excecute
   * @param[in] args - command arguments' values
   * @return int - result of stored class method.
   * @warning Method can throw exceptions
   */
  int execute(string const& commandName, args_order_t args = {}) {
    if (commands.find(commandName) == commands.end())
      throw exception("Unknown name");
  
    return commands[commandName]->execute(args);
  }
};