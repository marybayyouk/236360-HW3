#include <string>
#include "SymbolTable.h"
using std::vector;
///using std::string

vector<string> convertVectorToUpperCase(vector<string> toUpper);

enum Type {
    INT,
    BYTE,
    BOOL,
    STRING,
    VOID
};

const std::string typeToString[] = {"INT", "BYTE", "BOOL","STRING","VOID"};
//b3ref eno 5ara shel code 
Type stringToType(const std::string& typeStr) {
    if (typeStr == "INT") return Type::INT;
    if (typeStr == "BYTE") return Type::BYTE;
    if (typeStr == "BOOL") return Type::BOOL;
    if (typeStr == "STRING") return Type::STRING;
    return Type::VOID;
}


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

class Expression : public Node {
    Type type;
public:
    Expression();
    Expression(Node* exp, bool _); //𝐸𝑥𝑝 → Not Exp
    Expression(Node* exp); //𝐸𝑥𝑝 → 𝐿𝑃𝐴𝑅𝐸𝑁 𝐸𝑥𝑝 𝑅𝑃𝐴𝑅𝐸𝑁
    Expression(Call* call); //𝐸𝑥𝑝 → 𝐶𝑎𝑙𝑙
    Expression(Node* terminalExp); //𝐸𝑥𝑝 → 𝐼𝐷
    Expression(Node* exp, Type type); //𝐸𝑥𝑝 → 𝐿𝑃𝐴𝑅𝐸𝑁 𝑇𝑦𝑝𝑒 𝑅𝑃𝐴𝑅𝐸𝑁 𝐸𝑥𝑝
    Expression(Node* terminalExp, Type type); //Exp->BOOL/BYTE/INT/NUM/STRING
    Expression(Node* leftExp, Node* rightExp, string op); // Exp -> Exp And / Or Exp
    ~Expression() = default;
    Type getType() const { return type; }
    void setType(Type toSet) { type = toSet; }
};

class Call : public Node {
public:
    Call(Type type, Node* terminalID);
    ~Call() = default;
};

class Program : public Node {
public:
    Program() {};
    ~Program() = default;
};

class Statement : public Node {
public:
    Statement() {};
    Statement(std::string value); // Statement -> BREAK / CONTINUE
    Statement(Call * call); // Statement -> Call SC
    Statement(Type type,Node * id ); // Statement -> Type ID SC  
    Statement(Type type, Node * id, Expression * exp); // Statement -> Type ID Assign Exp SC
    Statement(Node * id, Expression * exp); // Statement -> ID Assign Exp SC
    Statement(Statments* Statments); // Statement -> { Statements }
    Statement(Expression* exp); // Statement -> IF ( Exp ) Statement
};

class Statments : public Node {
    public:
    Statments(Statement* Statement) : Node() {};
    Statments (Statement* Statement, Statments* Statments) : Node() {};
    ~Statments() = default;
};


