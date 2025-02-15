#include <cstdlib>
#include <iostream>
#include <map>
#include <functional>

#include "reader.hpp"

static std::string rep(std::string line);
static std::unique_ptr<MalType> read(std::string line);
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

static std::string rep(std::string line) {
  Mal::repl_env env = {
      {"+", [](const MalNumber &a, const MalNumber &b) { return a + b; }},
      {"-", [](const MalNumber &a, const MalNumber &b) { return a - b; }},
      {"*", [](const MalNumber &a, const MalNumber &b) { return a * b; }},
      {"/", [](const MalNumber &a, const MalNumber &b) { return a / b; }}};
  std::unique_ptr<MalType> ast = read(line);
  MalType *evaled_ast = ast->eval(env);
  if (evaled_ast != ast.get()) {
    ast.reset(evaled_ast);
  }
  return print(std::move(ast));
}

static std::unique_ptr<MalType> read(std::string line) {
  return read_str(line);
}

static std::string print(std::unique_ptr<MalType> ast) { return ast->as_str(); }
