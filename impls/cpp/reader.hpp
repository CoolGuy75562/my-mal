#ifndef READER_HPP
#define READER_HPP

#include "types.hpp"

class Reader {
public:
  Reader(std::vector<std::string> tokens);
  Reader();
  ~Reader();

  std::string next(void);
  std::string peek(void);

private:
  std::vector<std::string> tokens;
  unsigned int token_pos;
};

std::unique_ptr<MalType> read_str(std::string input);

#endif 
