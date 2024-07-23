#include "ProgramTypes.h"
#include "hw3_output.hpp"

extern int yylineno;
extern StackTable scopes;

using namespace std;


/////////// MAYBE WE NEED TO USE DYNAMIC CAST FOR RECRUSIVE RULES, EXAMPLE: EXP->EXP SC

vector<string> convertVectorToUpperCase(vector<string> toUpper) {
    vector<string> toRet;
    for (string name : toUpper) {
        toRet.push_back(upperCase(name));
    }
    return toRet;
}

Call::Call(Type type, Node* terminalID) : Node(terminalID->getValue()) {
    if (!scopes.isDefinedInProgram(terminalID->getValue())) {
        output::errorUndefFunc(yylineno, terminalID->getValue());
        exit(0);
    }
    if (!scopes.findSymbol(terminalID->getValue())->getIsFunction()) {
        output::errorUndefFunc(yylineno, terminalID->getValue());
        exit(0);
    }
    if(scopes.findSymbol(terminalID->getValue())->getNameType().names.size() > 0) {
        output::errorPrototypeMismatch(yylineno, terminalID->getValue());
        exit(0);
    }
    type = stringToType(scopes.findSymbol(terminalID->getValue())->getType());
    setValue(scopes.findSymbol(terminalID->getValue())->getName());
}

Statement::Statement(std::string value) : Node(value) {
    if (value == "BREAK") {
        if (!scopes.getScope()->getIsLoop()) {
            output::errorUnexpectedBreak(yylineno);
            exit(0);
        }
    } else if (value == "CONTINUE") {
        if (!scopes.getScope()->getIsLoop()) {
            output::errorUnexpectedContinue(yylineno);
            exit(0);
        }
    }
}

Statement::Statement(Call * call) : Node() {
    if (!scopes.isDefinedInProgram(call->getValue()) || !scopes.findSymbol(call->getValue())->getIsFunction()){
        output::errorUndefFunc(yylineno, call->getValue());
        exit(0);
    }
}

Statement::Statement(Type type, Node * id) {
    if (scopes.isDefinedInProgram(id->getValue())) {
        output::errorDef(yylineno, id->getValue());
        exit(0);
    }
    scopes.addSymbolToProgram(id->getValue(), false, typeToString[(int)type], {});
    setValue(typeToString[(int)type]);
}

bool LegalType(string typeOne, string typeTwo ) {
    if (typeOne == "INT" && typeTwo == "BYTE") {
        return true;
    } else if (typeOne == typeTwo) {
        return true;
    }
    // need to check 3rd legal assignment (byte) (int) with casting
    return false;
}

Statement::Statement(Type type, Node * id, Expression * exp) {
    if (scopes.isDefinedInProgram(id->getValue())) {
        output::errorDef(yylineno, id->getValue());
    }
    if (!LegalType(typeToString[(int)type], exp->getValue())) {
        output::errorMismatch(yylineno);
        exit(0);
    }
    setValue(typeToString[(int)type]);
    scopes.addSymbolToProgram(id->getValue(), false, typeToString[(int)type], {});
}
//Statement L Statement R 
Statement::Statement(Statments* Statments) {
    //open new scope
    scopes.pushScope(false, "");
}

Expression::Expression() : Node(""), type(VOID) {};

//𝐸𝑥𝑝 → 𝐿𝑃𝐴𝑅𝐸𝑁 𝐸𝑥𝑝 𝑅𝑃𝐴𝑅𝐸𝑁
Expression::Expression(Node *exp) : Node(exp->getValue()) {};

//𝐸𝑥𝑝 → 𝐶𝑎𝑙𝑙
Expression::Expression(Call* call) {
    setValue(call->getValue());
    setType(stringToType(scopes.findSymbol(call->getValue())->getType()));
}

//𝐸𝑥𝑝 → 𝐼𝐷
Expression::Expression(Node* terminalExp) {
    if (!scopes.isDefinedInProgram(terminalExp->getValue())){
        output::errorUndef(yylineno, terminalExp->getValue());
        exit(0);
    }
    setValue(terminalExp->getValue());
    setType(stringToType(scopes.findSymbol(terminalExp->getValue())->getType()));
}

//𝐸𝑥𝑝 → 𝐿𝑃𝐴𝑅𝐸𝑁 𝑇𝑦𝑝𝑒 𝑅𝑃𝐴𝑅𝐸𝑁 𝐸𝑥𝑝
Expression::Expression(Node* toExp, Type type) {
    Expression* exp = dynamic_cast<Expression *> (toExp);
    if(!LegalType(typeToString[int(exp->getType())], typeToString[int(type)])){
        output::errorMismatch(yylineno);
        exit(0);
    }
    setType(type);
    setValue(exp->getValue());
}

//Exp->BOOL/BYTE/INT/NUM
Expression::Expression(Node* terminalExp, Type type1): Node(terminalExp->getValue()) {
    type = type1;
    if((type == BYTE) && (stoi(terminalExp->getValue()) > 255)){
        output::errorByteTooLarge(yylineno, terminalExp->getValue());
        exit(0);
    }
}

