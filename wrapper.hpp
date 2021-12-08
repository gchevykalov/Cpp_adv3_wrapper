#pragma once

#include <vector>
#include <string>
#include <functional>
#include <map>

using std::vector;
using std::string;
using std::exception;

/**
 * Wrapper command's arguments struct
 */
struct args_t {
  vector<string> argsName;  ///< argument names
  vector<int> argsValue;    ///< argument values
};

using args_order_t = vector<std::pair<string, int>>;

/**
 * @brief Wrapper class
 * Wrapper for 'int(*func)(args...)' class'es methods
 */
class wrapper_t {
private:
  std::function<int(vector<int> const& args)> wmethod;  ///< stores the function calling the stored method
  vector<int> defaultValues;                            ///< default values of method parameters
  args_t methodArgs;                                    ///< arguments whose values are passed to the method
  std::map<string, int*> argsMap;                       ///< map for quick access to values

  /**
   * @brief Check are command arguments have similar names
   * @param[in] args - command arguments
   * @return true - if have similar names, false - if not
   */
  bool IsHaveSameName(args_order_t const& args) {
    for (int i = 0; i < args.size(); i++)
      for (int j = i + 1; j < args.size(); j++)
        if (args[i].first == args[j].first)
          return true;
    return false;
  }
public:
  wrapper_t() = delete;

  /**
   * @brief Templated wrapper_t constructor
   * @param[in] cls - class pointer
   * @param[in] method - class method pointer
   * @param[in] args - definition of argument names, their order and their default values
   */
  template<typename Class, typename... Args>
  wrapper_t(Class* cls, int(Class::* method)(Args...), args_order_t const& args) {
    if (cls == nullptr || method == nullptr)
      throw exception("Unexpected 'nullptr' from function's params");
    if (IsHaveSameName(args))
      throw exception("Two arguments have same names!");
    if (args.size() != sizeof...(Args))
      throw exception("Incorrect count of arguments");

    methodArgs.argsName = vector<string>(args.size());
    methodArgs.argsValue = vector<int>(args.size());
    defaultValues = vector<int>(args.size());
    int i = 0;
    for (auto& arg : args) {
      methodArgs.argsName[i] = arg.first;
      defaultValues[i] = arg.second;
      argsMap[args[i].first] = &methodArgs.argsValue[i];
      ++i;
    }

    wmethod = [this, cls, method](vector<int> const& funcArgs) {
      return callMethod(cls, method, funcArgs, std::make_index_sequence<sizeof...(Args)>{});
    };
  }
  
  /**
   * @brief Templated function calling the specified method
   * @param[in] cls - class pointer
   * @param[in] method - method, which stored into 'Class'
   * @return int - result of method
   */
  template<typename Class, typename Method, size_t... I>
  int callMethod(Class* cls, Method method, vector<int> const& args, std::index_sequence<I...>) {
    return (cls->*method)(args[I]...);
  }

  /**
   * @brief Execute wrappered class method
   * @param[in] args - new values of initialized arguments
   * @return int - result of stored class method.
   * @warning Can throw exception
   */
  int execute(args_order_t args) {
    methodArgs.argsValue = defaultValues;
    for (auto& arg : args)
      if (argsMap.find(arg.first) == argsMap.end())
        throw exception(("Unknown argument " + arg.first).c_str());
      else
        *argsMap[arg.first] = arg.second;

    return wmethod(methodArgs.argsValue);
  }
};