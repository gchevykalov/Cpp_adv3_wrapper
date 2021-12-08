#include "engine.hpp"
#include "testClass.hpp"
#include <iostream>

void WrapperInit_ArgsSameNames() {
  Test t;
  wrapper_t wrapper(&t, &Test::testMethod, { {"arg1", 0}, {"arg1", 0} });
}

void WrapperInit_NotEnoughtArgs() {
  Test t;
  wrapper_t wrapper(&t, &Test::testMethod, { {"arg1", 0}});
}

void WrapperInit_TooMuchArgs() {
  Test t;
  wrapper_t wrapper(&t, &Test::testMethod, { {"arg1", 0}, {"arg2", 0}, {"arg3", 0} });
}

void WrapperInit_CorrectInit() {
  Test t;
  wrapper_t wrapper(&t, &Test::testMethod, { {"arg1", 0}, {"arg2", 0} });
}

void EngineReg_NullptrWrapper() {
  engine_t engine;
  engine.registerCommand(nullptr, "command1");
}

void EngineReg_WrappersHaveSameNames() {
  engine_t engine;
  
  Test t;
  
  wrapper_t wrapper1(&t, &Test::testMethod, { {"arg1", 0}, {"arg2", 0} });
  wrapper_t wrapper2(&t, &Test::testMethod, { {"arg1", 0}, {"arg2", 0} });
  
  engine.registerCommand(&wrapper1, "command");
  engine.registerCommand(&wrapper2, "command");
}

void EngineExecute_IncorrectWrapperName() {
  engine_t engine;
  
  Test t;
  
  wrapper_t wrapper(&t, &Test::testMethod, { {"arg1", 0}, {"arg2", 0} });
  
  engine.registerCommand(&wrapper, "command");
  
  engine.execute("command1", {});
}

void EngineExecute_BadWrapperArgsName() {
  engine_t engine;

  Test t;

  wrapper_t wrapper(&t, &Test::testMethod, { {"arg1", 0}, {"arg2", 0} });

  engine.registerCommand(&wrapper, "command");

  engine.execute("command", { {"arg3", 0} });
}

void EngineExecute_CheckArgsOrder() {
  engine_t engine;

  Test t;

  wrapper_t wrapper(&t, &Test::testMethod, { {"arg1", 2}, {"arg2", 5} });

  engine.registerCommand(&wrapper, "command");

  if (engine.execute("command", {}) != t.testMethod(2, 5))
    throw exception("Incorrect order");
}

void runTests() {
  try {
    WrapperInit_ArgsSameNames();
  }
  catch (exception e) {
    std::cerr << "1: " << e.what() << std::endl;
  }
  try {
    WrapperInit_NotEnoughtArgs();
  }
  catch (exception e) {
    std::cerr << "2: " << e.what() << std::endl;
  }
  try {
    WrapperInit_TooMuchArgs();
  }
  catch (exception e) {
    std::cerr << "3: " << e.what() << std::endl;
  }
  try {
    WrapperInit_CorrectInit();
  }
  catch (exception e) {
    std::cerr << "4: " << e.what() << std::endl;
  }
  try {
    EngineReg_NullptrWrapper();
  }
  catch (exception e) {
    std::cerr << "5: " << e.what() << std::endl;
  }
  try {
    EngineReg_WrappersHaveSameNames();
  }
  catch (exception e) {
    std::cerr << "6: " << e.what() << std::endl;
  }
  try {
    EngineExecute_IncorrectWrapperName();
  }
  catch (exception e) {
    std::cerr << "7: " << e.what() << std::endl;
  }
  try {
    EngineExecute_BadWrapperArgsName();
  }
  catch (exception e) {
    std::cerr << "8: " << e.what() << std::endl;
  }
  try {
    EngineExecute_CheckArgsOrder();
  }
  catch (exception e) {
    std::cerr << "9: " << e.what() << std::endl;
  }
}

int main() {
  Test cls;

  wrapper_t wrapper(&cls, &Test::testMethod, { {"arg1", 0}, {"arg2", 0} });

  engine_t engine;

  engine.registerCommand(&wrapper, "command");

  std::cout << engine.execute("command", { {"arg1", 5}, {"arg2", 4} }) << std::endl;

  runTests();

  return 0;
}
