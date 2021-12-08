# CMakeList.txt: проект CMake для wrapper; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

project ("wrapper")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (wrapper "main.cpp" "wrapper.hpp" "engine.hpp"  "testClass.hpp")

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
