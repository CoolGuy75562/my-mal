#include <cstdlib>
#include <iostream>
#include <string>

static void rep(std::string line);
static std::string read(std::string line);
static std::string eval(std::string line);
static std::string print(std::string line);

int main(void) {

  std::string line;
  for (;;) {
    std::cout << "user> ";
    std::getline(std::cin, line);
    if (!std::cin) {
      break;
    }
    std::cout << line << std::endl;
    rep(line);
  }
  
  return EXIT_SUCCESS;
}

static void rep(std::string line) { print(eval(read(line))); }
static std::string read(std::string line) { return line; }
static std::string eval(std::string line) { return line; }
static std::string print(std::string line) { return line; }
