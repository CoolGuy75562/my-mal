#include <stdexcept>
#include <numeric>

#include "types.hpp"

MalType::MalType() {}
MalType::~MalType() {}

MalContainer::~MalContainer() {}
MalContainer::MalContainer(char ob, char cb) : open_bracket(ob), close_bracket(cb) {}

MalList::MalList() : MalContainer('(', ')') {}
MalList::~MalList() {}

MalVector::MalVector() : MalContainer('[', ']') {}
MalVector::~MalVector() {}

MalHashMap::MalHashMap() : MalContainer('{', '}') {}
MalHashMap::~MalHashMap() {}

MalNumber::MalNumber(std::string token) : data(std::stoi(token)) {}
MalNumber::MalNumber(int data) : data(data) {}
MalNumber::MalNumber(const MalNumber &other) : data(other.data) {}
MalNumber::~MalNumber() {}

MalSymbol::MalSymbol(std::string token) : data(token) {}
MalSymbol::~MalSymbol() {}

MalNil::MalNil() {}
MalNil::~MalNil() {}

MalTrue::MalTrue() {}
MalTrue::~MalTrue() {}

MalFalse::MalFalse() {}
MalFalse::~MalFalse() {}

MalType *MalType::eval(const Mal::repl_env &env) { return this; }
std::string MalType::as_str(void) { return "Something\'s wrong..."; }

void MalContainer::append(MalType *item) {
  data.push_back(std::unique_ptr<MalType>(item));
}

MalType *MalContainer::eval(const Mal::repl_env &env) { return this; }

std::string MalContainer::as_str(void) {
  if (data.empty()) {
    return std::string{open_bracket} + std::string{close_bracket};
  }

  std::string out = {open_bracket};
  for (auto &item : data) {
    out += item->as_str();
    out += ' ';
  }
  out.back() = close_bracket;
  return out;
}


MalType *MalList::eval(const Mal::repl_env &env) {
  if (data.empty()) {
    return this;
  }

  // eval everything
  for (auto &item : data) {
    MalType *evaled_ast = item->eval(env);
    if (evaled_ast != item.get()) {
      item.reset(evaled_ast);
    }
  }
  
  auto op = data.begin()->get();
  Mal::b_oper f = env.at(op->as_str().c_str());

  std::vector<MalNumber> nums;
  nums.reserve(data.size() - 1);
  for (auto iter = data.begin() + 1; iter != data.end(); ++iter) {
    MalNumber *number = dynamic_cast<MalNumber *>((*iter).get());
    if (number == nullptr) {
      throw std::runtime_error("Not a number...");
    }
    nums.push_back(*number);
  }

  const MalNumber init_val = nums.at(0);
  return new MalNumber(std::accumulate(nums.begin() + 1, nums.end(), init_val, f));
}

MalType *MalVector::eval(const Mal::repl_env &env) {
  if (data.empty()) {
    return this;
  }

  for (auto &item : data) {
    MalType *evaled_ast = item->eval(env);
    if (evaled_ast != item.get()) {
      item.reset(evaled_ast);
    }
  }
  return this;
}

MalType *MalHashMap::eval(const Mal::repl_env &env) {
  if (data.empty()) {
    return this;
  }

  for (auto &item : data) {
    MalType *evaled_ast = item->eval(env);
    if (evaled_ast != item.get()) {
      item.reset(evaled_ast);
    }
  }
  return this;
}


MalNumber operator+(const MalNumber &a, const MalNumber &b) {
  return MalNumber(a.data + b.data);
}
MalNumber operator/(const MalNumber &a, const MalNumber &b) {
  return MalNumber(a.data / b.data);
}
MalNumber operator*(const MalNumber &a, const MalNumber &b) {
  return MalNumber(a.data * b.data);
}
MalNumber operator-(const MalNumber &a, const MalNumber &b) {
  return MalNumber(a.data - b.data);
}

std::string MalNumber::as_str(void) { return std::to_string(data); }

std::string MalSymbol::as_str(void) { return data; }

MalType *MalSymbol::eval(const Mal::repl_env &env) {
  if (!env.count(data)) {
    throw std::runtime_error("not found");
  }
  return this;
}

std::string MalNil::as_str(void) {
  return "nil";
}

std::string MalTrue::as_str(void) { return "true"; }

std::string MalFalse::as_str(void) { return "false"; }
