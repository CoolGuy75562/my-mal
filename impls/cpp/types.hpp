#ifndef TYPES_HPP
#define TYPES_HPP

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

class MalType;
class MalNumber;
class MalSymbol;
class MalList;

namespace Mal {
  
using b_oper = std::function<MalNumber(const MalNumber &, const MalNumber &)>;

using repl_env = std::map<std::string, b_oper>;
  
  typedef enum Type { LIST, VECTOR, HASH_MAP, SYMBOL, NUMBER, NIL, TRUE, FALSE } Type;
} // namespace Mal

class MalType {
public:
  MalType();
  virtual ~MalType();
  virtual MalType *eval(const Mal::repl_env &env);
  virtual std::string as_str(void);
};

class MalContainer : public MalType {
public:
  MalContainer(char ob, char cb);
  virtual ~MalContainer();

  void append(MalType *item);
  std::string as_str(void) override;
  virtual MalType *eval(const Mal::repl_env &env) override;

protected:
  char open_bracket, close_bracket;
  std::vector<std::unique_ptr<MalType>> data;
};


class MalList : public MalContainer {
public:
  MalList();
  ~MalList();

  MalType *eval(const Mal::repl_env &env) override;
};

class MalVector : public MalContainer {
public:
  MalVector();
  ~MalVector();

  MalType *eval(const Mal::repl_env &env) override;
};

class MalHashMap : public MalContainer {
public:
  MalHashMap();
  ~MalHashMap();

  MalType *eval(const Mal::repl_env &env) override;
};

class MalNumber : public MalType {

  friend MalNumber operator+(const MalNumber &a, const MalNumber &b);
  friend MalNumber operator-(const MalNumber &a, const MalNumber &b);
  friend MalNumber operator*(const MalNumber &a, const MalNumber &b);
  friend MalNumber operator/(const MalNumber &a, const MalNumber &b);

public:
  MalNumber(std::string token);
  MalNumber(int data);
  MalNumber(const MalNumber &other);
  MalNumber();
  ~MalNumber();

  std::string as_str(void) override;

private:
  int data;
};

class MalSymbol : public MalType {
public:
  MalSymbol(std::string token);
  ~MalSymbol();

  MalType *eval(const Mal::repl_env &env) override;
  std::string as_str(void) override;

private:
  std::string data;
};

class MalNil : public MalType {
public:
  MalNil();
  ~MalNil();

  std::string as_str(void) override;
};

class MalTrue : public MalType {
public:
  MalTrue();
  ~MalTrue();

  std::string as_str(void) override;
};

class MalFalse : public MalType {
public:
  MalFalse();
  ~MalFalse();

  std::string as_str(void) override;
};

#endif
