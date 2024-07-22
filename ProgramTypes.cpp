#include "ProgramTypes.h"
#include "hw3_output.hpp"

extern int yylineno;
extern StackTable scopes;

using namespace std;

Call::Call(Node* terminalID) : Node(terminalID->getValue()) {
    if (!scopes.isDefinedInProgram(terminalID->getValue())) {
        output::errorUndefFunc(yylineno, terminalID->getValue());
        exit(0);
    }
    if (!scopes.findSymbol(terminalID->getValue())->getIsFunction()) {
        output::errorUndefFunc(yylineno, terminalID->getValue());
        exit(0);
    }
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





