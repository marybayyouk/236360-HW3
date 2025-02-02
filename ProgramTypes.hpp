#include <string>
#include <iostream>
#include "hw3_output.hpp"
#include "SymbolTable.hpp"
#define YYSTYPE Node*


vector<string> convertVectorToUpperCase(vector<string> toUpper);
bool LegalType(string typeOne, string typeTwo);

class Node {
    std::string value;
    std::string type;
public:
    //Node() : value(""),type("") {};
    //Node(char* value, char* type = "") : value(value), type(type) { };
    Node(std::string value = "", std::string type = "" ) : value(value), type(type) { };
    std::string getValue() const { return value; }
    std::string getType() const { return type; }
    Node(Node* node) : value(node->getValue()), type(node->getType()) {};
    void setValue(std::string value) { this->value = value; }
    void setType(std::string type) { this->type = type; }
    virtual ~Node() {};
};

class Type : public Node {
public:
    Type(std::string type) : Node("",type) {    
        };
};


class iD : public Node {
public:
    iD(Node* id) : Node(id->getValue(), "ID") {
    }
};

class Exp;

class Call : public Node {
public:
    Call(Node* terminalId, Exp* exp);
    ~Call() = default;
};

class Exp : public Node {
    bool isFunction;
public:
    Exp();
    Exp(int mode, Node* exp); //𝐸𝑥𝑝 → 𝐿𝑃𝐴𝑅𝐸𝑁 𝐸𝑥𝑝 𝑅𝑃𝐴𝑅𝐸𝑁
    Exp(Exp* exp, bool _ = true); //𝐸𝑥𝑝 → Not Exp
    Exp(Node* terminalExp, string rule); //𝐸𝑥𝑝 → 𝐼𝐷/𝐶𝑎𝑙𝑙
    Exp(Type* type, Exp* exp); //𝐸𝑥𝑝 → 𝐿𝑃𝐴𝑅𝐸𝑁 𝑇𝑦𝑝𝑒 𝑅𝑃𝐴𝑅𝐸𝑁 𝐸𝑥𝑝
    Exp(Node* leftExp, Node* rightExp, const string op); // Exp -> Exp And/Or/Relop/Binop Exp
    Exp(string value, string type) : Node(value,type) {} ///FOR CHILD C'TORS
    ~Exp() = default;
};

class Bool: public Exp {
public:
    Bool(Node* expression) : Exp(expression->getValue(), "BOOL") {}
};

class Num: public Exp 
{
public:

    Num(Node* exp) : Exp(exp->getValue(), "INT") {}
};


class NumB: public Exp {
public:
    NumB(Node* expression);
};

class String: public Exp {
public:
    String(Node* exp) : Exp(exp->getValue(), "STRING") {}
};


class Statement : public Node {
public:
    Statement() {};
    Statement(Statement* Statement) {};
    Statement(Node* BCNode); // Statement -> BREAK / CONTINUE
    Statement(Call * call); // Statement -> Call SC
    Statement(Type* type, Node * id); // Statement -> Type ID SC  
    Statement(Type* type, Node * id, Exp * exp, bool flag); // Statement -> Type ID Assign Exp SC
    Statement(int x, int y, Exp* exp); // Statement -> if exp Else/Noelse
    Statement(Node * id, Exp * exp); // Statement -> ID Assign Exp SC
    //Statement(Statments* Statments); // Statement -> { Statements }
};

class Statements : public Node {
    public:
    Statements(Statement* Statement) {};
    Statements (Statements* Statements, Statement* Statment){};
    ~Statements() = default;
};

class Program : public Node {
public:
    Program() {};
    ~Program() = default;
};




