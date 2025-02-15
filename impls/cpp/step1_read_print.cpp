#include <cstdlib>
#include <iostream>

#include "reader.hpp"

static std::string rep(std::string line);
static std::unique_ptr<MalType> read(std::string line);
static std::unique_ptr<MalType> eval(std::unique_ptr<MalType> ast);
static std::string print(std::unique_ptr<MalType> ast);

int main(void) {

  std::string line;
  for (;;) {
    std::cout << "user> ";
    std::getline(std::cin, line);
    if (!std::cin) {
      break;
    }
    try {
      std::cout << rep(line) << std::endl;
    } catch (std::runtime_error &e) {
      std::cout << e.what() << std::endl;
    }
  }
  
  return EXIT_SUCCESS;
}

static std::string rep(std::string line) { return print(eval(read(line))); }

static std::unique_ptr<MalType> read(std::string line) { return read_str(line); }

static std::unique_ptr<MalType> eval(std::unique_ptr<MalType> ast) { return ast; }

static std::string print(std::unique_ptr<MalType> ast) {
  return ast->as_str();
}
