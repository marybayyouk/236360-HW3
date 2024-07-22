#include <string>
#include "SymbolTeble.h"

enum class Type {
    INT,
    BYTE,
    BOOL,
    STRING,
    VOID
};
class Node {
    std::string value;
public:
    Node() {};
    Node(std::string value) : value(value) {
    };
    std::string getValue() const { return value; }
    void setValue(std::string value) { this->value = value; }
    virtual ~Node() {};
    virtual void print() = 0;
};

class Call : public Node {
public:
    Call(Node* terminalID);
    ~Call() = default;
};


enum  Type {INT, BYTE, BOOL};
const std::string typeToString[] = {"INT", "BYTE", "BOOL"};

class Program : public Node {
public:
    Program() {};
    ~Program() = default;
};

class Statement : public Node {
public:
    Statement() {};
    Statement(std::string value);
    Statement(Call * call);
    Statement(Type type,Node * id );
    Statement(Type type, Node * id, Expression * exp);    
    Statement(Statments* Statments);
};

class Statments : public Node {
    public:
    Statments(Statement* Statement) : Node() {};
    Statments (Statement* Statement, Statments* Statments) : Node() {};
    ~Statments() = default;
};

class Expression : public Node {
public:
    Expression() {};
    ~Expression() = default;
};


