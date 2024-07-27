#include "ProgramTypes.hpp"

extern int yylineno;
extern StackTable stackTable;
using namespace std;

vector<string> convertVectorToUpperCase(vector<string> toUpper) {
    vector<string> toRet;
    for (string name : toUpper) {
        toRet.push_back(upperCase(name));
    }
    return toRet;
}

bool LegalType(string typeOne, string typeTwo) {
    if ((typeOne == "INT" && typeTwo == "BYTE") || (typeOne == typeTwo)) {
        return true;
    } 
    return false;
}

bool isLegalFun(string func, bool functionType) {
    if (func == "print" || func == "printi" || func == "readi") {
        if (!functionType) {
            return false;
        }
    }
    return true;
}

//////////////////////////////////////////Exp//////////////////////////////////////////

// 𝐸𝑥𝑝 → 𝑁𝑜𝑡 𝐸𝑥𝑝
Exp::Exp(Exp* exp, bool _) : Node(exp->getValue(), "") {
    //Exp* Exp = dynamic_cast<Exp*> (exp);
    if (_ == false && !stackTable.isDefinedInProgram(exp->getValue())){
        output::errorUndef(yylineno, exp->getValue());
    }
    if (exp->getType() != "BOOL") {
        output::errorMismatch(yylineno);
        exit(0);
    }
    setType("BOOL");
}

// 𝐸𝑥𝑝 → 𝐼𝐷/CALL
Exp::Exp(Node* terminalExp, string rule) {
    if (rule == "ID") {
        if (!stackTable.isDefinedInProgram(terminalExp->getValue())){
        output::errorUndef(yylineno, terminalExp->getValue());
        exit(0);
        }
        setValue(terminalExp->getValue());
        setType(stackTable.findSymbol(terminalExp->getValue())->getType());
    } 
    else if (rule == "CALL") {
        setValue(terminalExp->getValue());
        setType(stackTable.findSymbol(terminalExp->getValue())->getType());
        setIsFunc(true);
    }
}

// 𝐸𝑥𝑝 → 𝐿𝑃𝐴𝑅𝐸𝑁 𝑇𝑦𝑝𝑒 𝑅𝑃𝐴𝑅𝐸𝑁 𝐸𝑥𝑝
Exp::Exp(Type* type, Exp* exp) {
    if((exp->getType() != "INT" && exp->getType() != "BYTE") || (type->getType() != "INT" && type->getType() != "BYTE")){
        output::errorMismatch(yylineno);
        exit(0);
    }
    setType(type->getType());
    setValue(exp->getValue());
}

// Exp -> Exp And/Or/Relop/Binop Exp
Exp::Exp(Node* leftExp, Node* rightExp, const string op) {
    Exp* left = dynamic_cast<Exp*>(leftExp);
    Exp* right = dynamic_cast<Exp*>(rightExp);
    string lType = left->getType();
    string rType = right->getType();

    if (op == "AND" || op == "OR") {
        if (lType != "BOOL" || rType != "BOOL") {
            output::errorMismatch(yylineno);
            exit(0);
        } 
        setType("BOOL"); 
    } 
    else { ///IT IS RELOP OR BINOP
        if ((lType != "INT" && lType != "BYTE") || (rType != "INT" && rType != "BYTE")) {
            output::errorMismatch(yylineno);
            exit(0);
        }
        if (op == "BINOP"){
            if (lType == "BYTE" && rType == "BYTE") {
                setType("BYTE");
            } 
            else { setType("INT"); }
        }
        if (op == "RELOP") {
            setType("BOOL");
        }
    }
}
////////////////////////////////////////NumB////////////////////////////////////////////////////
NumB::NumB(Node* expression) : Exp(expression->getValue(), "BYTE") {
        if (stoi(expression->getValue()) >= 256) {
            output::errorByteTooLarge(yylineno, expression->getValue());
            exit(0);
        }
    }

//////////////////////////////////////////Call//////////////////////////////////////////
// Call -> ID LPAREN EXP RPAREN
Call::Call(Node* terminalID, Exp* exp) : Node(terminalID->getValue(), "") {
    if (!(terminalID->getValue() == "print") && !(terminalID->getValue() == "printi") && !(terminalID->getValue() == "readi")) {
        output::errorUndefFunc(yylineno, terminalID->getValue());
        exit(0);
    }
    if ((terminalID->getValue() == "print")) {
        if (exp->getType() != "STRING") {
            output::errorPrototypeMismatch(yylineno,terminalID->getValue(), "STRING");
            exit(0);
        }
    }
    else if (terminalID->getValue() == "printi") {
        if (exp->getType() != "BYTE" && exp->getType() != "INT") {
            output::errorPrototypeMismatch(yylineno,terminalID->getValue(), "INT");
            exit(0);
        }
    }
    else { //MUST BE READI FUNCTION
        if(exp->getType() != "BYTE" && exp->getType() != "INT") {
            output::errorPrototypeMismatch(yylineno,terminalID->getValue(), "INT");
            exit(0);
        }
    }
    setType(stackTable.findSymbol(terminalID->getValue())->getType());
    setValue(stackTable.findSymbol(terminalID->getValue())->getName());
}

//////////////////////////////////////////Statement//////////////////////////////////////////
//Statement -> Type ID SC ----TESTED----
Statement::Statement(Type* type, Node * id) {
    if (stackTable.isDefinedInProgram(id->getValue())) {
        output::errorDef(yylineno, id->getValue());
        exit(0);
    }
    id->setType(type->getType());
    stackTable.addSymbolToProgram(id->getValue(), false, type->getType(), {});
    //setValue(type->getValue());
}

// Statement -> Type ID Assign Exp SC  ----TESTED---
Statement::Statement(Type* type, Node * id, Exp * exp){

    if (stackTable.isDefinedInProgram(id->getValue())) {
        output::errorDef(yylineno, id->getValue());
        exit(0);
    }
    if (!isLegalFun(exp->getValue(), exp->isFunc())) {
        output::errorUndef(yylineno, exp->getValue());
        exit(0);
    }   
    if (!LegalType(type->getType(), exp->getType())) {
        output::errorMismatch(yylineno);
        exit(0);
    }
    stackTable.addSymbolToProgram(id->getValue(), false, type->getType(), {});
    id->setType(type->getType());
}

// Statement -> ID Assign Exp SC ----Tested-----
Statement::Statement(Node * id, Exp * exp) {
    if (!stackTable.isDefinedInProgram(id->getValue())) {
        output::errorUndef(yylineno, id->getValue());
        exit(0);
    }
    if (!LegalType((stackTable.findSymbol(id->getValue()))->getType(), exp->getType())) {
        output::errorMismatch(yylineno);
        exit(0);
    }
    string expV = exp->getValue();
    if (!isLegalFun(exp->getValue(), exp->isFunc())) {
        output::errorUndef(yylineno, exp->getValue());
        exit(0);
    }
}

// Statement -> Call SC 
Statement::Statement(Call * call) {};

// Statement -> IF|IF-ELSE|WHILE LP EXP RP SS -----Tested-----
Statement::Statement(const string cond, Exp* exp) {
    //cout << exp->getType() << endl;
    if (exp->getType() != "BOOL") {
            //cout << "here" << endl;
            output::errorMismatch(yylineno);
            exit(0);
        }
}

// Statement -> BREAK / CONTINUE 
Statement::Statement(Node* BKNode) : Node(BKNode->getValue(),"") {
    bool loop=false;
    for(SymbolTable* sym: stackTable.scopes) {
        if (sym->getIsLoop()){
            loop = true;
        }
    }
    if (BKNode->getValue() == "continue") {
        if (!loop) {
            output::errorUnexpectedContinue(yylineno);
            exit(0);
        }
    }
    else if (BKNode->getValue() == "break") {
        if (!loop) {
            output::errorUnexpectedBreak(yylineno);
            exit(0);
        }
    } 
}

