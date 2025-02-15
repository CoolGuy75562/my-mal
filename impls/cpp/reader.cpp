#include <regex>

#include "reader.hpp"

static std::vector<std::string> tokenize(std::string input);
static MalType *read_form(Reader &reader);
static MalContainer *read_container(Reader &reader, Mal::Type type);
static MalType *read_atom(Reader &reader);

Reader::Reader(std::vector<std::string> tokens)
    : tokens(tokens), token_pos(0) {}
Reader::~Reader() {}

std::string Reader::next(void) {
  if (token_pos >= tokens.size() - 1) {
    throw std::runtime_error("EOF");
  }
  return tokens.at(++token_pos);
}
std::string Reader::peek(void) { return tokens.at(token_pos); }

std::unique_ptr<MalType> read_str(std::string input) {
  Reader reader(tokenize(input));
  return std::unique_ptr<MalType>(read_form(reader));
}

static std::vector<std::string> tokenize(std::string input) {
  std::regex reg_exp("[\\s ,]*(~@|[\\[\\]{}()'`~@]|\"(?:[\\\\].|[^\\\\\"])*\"?|;.*|[^\\s \\[\\]{}()'\"`~@,;]*)");
  std::sregex_iterator iter(input.begin(), input.end(), reg_exp), end;
  std::vector<std::string> tokens;

  while (iter != end) {
    std::smatch match = *iter;
    if (match.size() > 1) {
      std::string token = match[1].str();
      if (!token.empty()) {
        tokens.push_back(token);
      }
    }
    ++iter;
  }
  return tokens;
}

static MalType *read_form(Reader &reader) {
  std::string token = reader.peek();
  switch (token.front()) {
  case '(':
    return read_container(reader, Mal::LIST);
  case '[':
    return read_container(reader, Mal::VECTOR);
  case '{':
    return read_container(reader, Mal::HASH_MAP);
  default:
    return read_atom(reader);
  }
}

static MalContainer *read_container(Reader &reader, Mal::Type type) {

  MalContainer *container;
  char cb;
  switch (type) {
  case Mal::LIST:
    container = new MalList();
    cb = ')';
    break;
  case Mal::VECTOR:
    container = new MalVector();
    cb = ']';
    break;
  case Mal::HASH_MAP:
    container = new MalHashMap();
    cb = '}';
    break;
  default:
    __builtin_unreachable();
  }
  while (reader.next().front() != cb) {
    container->append(read_form(reader));
  }
  return container;
}

static MalType *read_atom(Reader &reader) {
  std::string token = reader.peek();
  if (!token.compare("-")) {
    return new MalSymbol(token);
  }

  if (!token.compare("nil")) {
    return new MalNil();
  }

  if (!token.compare("true")) {
    return new MalTrue();
  }

  if (!token.compare("false")) {
    return new MalFalse();
  }
  
  auto iter = token.begin();
  if (*iter == '-' || isdigit(*iter)) {
    while (++iter != token.end()) {
      if (!isdigit(*iter)) {
        return new MalSymbol(token);
      }
    }
    return new MalNumber(token);
  }
  return new MalSymbol(token);
}
