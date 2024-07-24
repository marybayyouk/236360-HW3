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
    //Node(char* value, char* type = "") : value(value), type(type) { };
    Node(std::string value,std::string type = "" ) : value(value), type(type) { };
    std::string getValue() const { return value; }
    std::string getType() const { return type; }
    void setValue(std::string value) { this->value = value; }
    void setType(std::string type) { this->type = type; }
    virtual ~Node() {};
};

class Exp : public Node {
public:
    Exp();
    Exp(Exp* exp, bool _); //𝐸𝑥𝑝 → Not Exp
    Exp(Node* exp); //𝐸𝑥𝑝 → 𝐿𝑃𝐴𝑅𝐸𝑁 𝐸𝑥𝑝 𝑅𝑃𝐴𝑅𝐸𝑁
    Exp(Call* call); //𝐸𝑥𝑝 → 𝐶𝑎𝑙𝑙
    Exp(Node* terminalExp, int mode); //𝐸𝑥𝑝 → 𝐼𝐷
    Exp(Node* exp, Type* type); //𝐸𝑥𝑝 → 𝐿𝑃𝐴𝑅𝐸𝑁 𝑇𝑦𝑝𝑒 𝑅𝑃𝐴𝑅𝐸𝑁 𝐸𝑥𝑝
    Exp(Node* leftExp, Node* rightExp, string op); // Exp -> Exp And/Or/Relop/Binop Exp
    Exp(string value, string type) : Node(value,type) {} ///FOR CHILD C'TORS
    ~Exp() = default;
};

class Bool: public Exp {
public:
    Bool(Node* expression) : Exp(expression->getValue(), "bool") {}
};

class Num: public Exp 
{
public:

    Num(Node* exp) : Exp(exp->getValue(), "int") {}
};


class NumB: public Exp {
public:
    NumB(Node* expression);
};

class String: public Exp {
public:
    String(Node* exp) : Exp(exp->getValue(), "string") {}
};

class Call : public Node {
public:
    Call(string type, Node* terminalID);
    ~Call() = default;
};

class Type : public Node {
public:
    Type(std::string type) : Node("",type) {};
};


class Statement : public Node {
public:
    Statement() {};
    Statement(Statement* Statement) {};
    Statement(Node* BCNode); // Statement -> BREAK / CONTINUE
    Statement(Call * call); // Statement -> Call SC
    Statement(Type* type, Node * id); // Statement -> Type ID SC  
    Statement(Type* type, Node * id, Exp * exp, bool flag); // Statement -> Type ID Assign Exp SC
    Statement(Node * id, Exp * exp); // Statement -> ID Assign Exp SC
    //Statement(Statments* Statments); // Statement -> { Statements }
    Statement(string str, Exp* exp); // Statement -> IF ( Exp ) Statement
};

class Statements : public Node {
    public:
    Statements(Statement* Statement) {};
    Statements (Statement* Statement, Statements* Statments){};
    ~Statements() = default;
};

class Program : public Node {
public:
    Program() {};
    ~Program() = default;
};



