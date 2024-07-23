#include <string>
#include "SymbolTable.h"
using std::vector;

vector<string> convertVectorToUpperCase(vector<string> toUpper);
bool LegalType(string typeOne, string typeTwo);

class Node {
    std::string value;
    std::string type;
public:
    Node() : value(""),type("") {};
    Node(std::string value,std::string type) : value(value), type(type) { };
    std::string getValue() const { return value; }
    std::string getType() const { return type; }
    void setValue(std::string value) { this->value = value; }
    void setType(std::string type) { this->type = type; }
    virtual ~Node() {};
    virtual void print() = 0;
};

class Type : public Node {
public:
    Type(std::string type) : Node("",type) {};
};

class Expression : public Node {
public:
    Expression();
    Expression(Node* exp, bool _); //𝐸𝑥𝑝 → Not Exp
    Expression(Node* exp); //𝐸𝑥𝑝 → 𝐿𝑃𝐴𝑅𝐸𝑁 𝐸𝑥𝑝 𝑅𝑃𝐴𝑅𝐸𝑁
    Expression(Call* call); //𝐸𝑥𝑝 → 𝐶𝑎𝑙𝑙
    Expression(Node* terminalExp); //𝐸𝑥𝑝 → 𝐼𝐷
    Expression(Node* exp, string type); //𝐸𝑥𝑝 → 𝐿𝑃𝐴𝑅𝐸𝑁 𝑇𝑦𝑝𝑒 𝑅𝑃𝐴𝑅𝐸𝑁 𝐸𝑥𝑝
    Expression(Node* terminalExp, string type); //Exp->BOOL/BYTE/INT/NUM/STRING
    Expression(Node* leftExp, Node* rightExp, string op); // Exp -> Exp And / Or Exp
    ~Expression() = default;
};

class Call : public Node {
public:
    Call(string type, Node* terminalID);
    ~Call() = default;
};

class Statement : public Node {
public:
    Statement() {};
    Statement(std::string value); // Statement -> BREAK / CONTINUE
    Statement(Call * call); // Statement -> Call SC
    Statement(string type,Node * id); // Statement -> Type ID SC  
    Statement(string type, Node * id, Expression * exp); // Statement -> Type ID Assign Exp SC
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

class Program : public Node {
public:
    Program() {};
    ~Program() = default;
};



