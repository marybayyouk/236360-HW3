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
    if (typeOne != "INT" && typeOne != "BYTE") {
        return false;
    } else if (typeTwo != "INT" && typeTwo != "BYTE") {
        return false;
    }
    return true;
}

//////////////////////////////////////////Exp//////////////////////////////////////////
Exp::Exp() : Node("","VOID") {};

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

// 𝐸𝑥𝑝 → 𝐿𝑃𝐴𝑅𝐸𝑁 𝐸𝑥𝑝 𝑅𝑃𝐴𝑅𝐸𝑁
Exp::Exp(int mode, Node *exp) : Node(exp->getValue(), "") {};


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
        isFunction = true;
    }
}
    

// 𝐸𝑥𝑝 → 𝐿𝑃𝐴𝑅𝐸𝑁 𝑇𝑦𝑝𝑒 𝑅𝑃𝐴𝑅𝐸𝑁 𝐸𝑥𝑝
Exp::Exp(Type* type, Exp* exp) {
    //Exp* exp = dynamic_cast<Exp *> (toExp);
    if(!LegalType(exp->getType(), type->getType())){
        output::errorMismatch(yylineno);
        exit(0);
    }
    setType(type->getType());
    setValue(exp->getValue());
}

// // Exp->BOOL/BYTE/INT/NUM/STRING
// Exp::Exp(Node* terminalExp, Type* type) : Node(terminalExp->getValue(), type->getType()) {
//     if((type->getType() == "BYTE") && (stoi(terminalExp->getValue()) > 255)){
//         output::errorByteTooLarge(yylineno, terminalExp->getValue());
//         exit(0);
//     }
// }


// Exp -> Exp And/Or/Relop/Binop Exp
Exp::Exp(Node* leftExp, Node* rightExp, const string op) {
    Exp* left = dynamic_cast<Exp *> (leftExp);
    Exp* right = dynamic_cast<Exp *> (rightExp);
    string lType = left->getType();
    string rType = right->getType();

    if (op == "AND" || op == "OR") {
        if (lType != "BOOL" || rType != "BOOL") {
            output::errorMismatch(yylineno);
            exit(0);
        } 
        setType("BOOL"); 
    } else { ///IT IS RELOP OR BINOP
        cout<<lType<<" l type" <<endl;
        cout <<rType<<"r type" << endl;
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
            output::errorPrototypeMismatch(yylineno,terminalID->getValue());
            exit(0);
        }
    }
    else if (terminalID->getValue() == "printi") {
        if (exp->getType() != "BYTE" && exp->getType() != "INT") {
            output::errorPrototypeMismatch(yylineno,terminalID->getValue());
            exit(0);
        }
    }
    else { //MUST BE READI FUNCTION
        if(exp->getType() != "BYTE" && exp->getType() != "INT") {
            output::errorPrototypeMismatch(yylineno,terminalID->getValue());
            exit(0);
        }
    }
    setType(stackTable.findSymbol(terminalID->getValue())->getType());
    setValue(stackTable.findSymbol(terminalID->getValue())->getName());
}

//////////////////////////////////////////Statement//////////////////////////////////////////
// Statement -> BREAK / CONTINUE
Statement::Statement(Node* BKNode) : Node(BKNode->getValue(),"") {
    if (BKNode->getValue() == "BREAK") {
        if (!stackTable.getScope()->getIsLoop()) {
            output::errorUnexpectedBreak(yylineno);
            exit(0);
        }
    } 
    else if (BKNode->getValue() == "CONTINUE") {
        if (!stackTable.getScope()->getIsLoop()) {
            output::errorUnexpectedContinue(yylineno);
            exit(0);
        }
    }
}

//Statement -> Type ID SC 
Statement::Statement(Type* type, Node * id) {
    if (stackTable.isDefinedInProgram(id->getValue())) {
        output::errorDef(yylineno, id->getValue());
        exit(0);
    }
    stackTable.addSymbolToProgram(id->getValue(), false, type->getType(), {});
    setValue(type->getValue());
}

// Statement -> Type ID Assign Exp SC
Statement::Statement(Type* type, Node * id, Exp * exp, bool flag){
    if (flag) {
        if (stackTable.isDefinedInProgram(id->getValue())) {
            output::errorDef(yylineno, id->getValue());
            exit(0);
        }
         if (!LegalType(type->getType(), exp->getType())) {
            output::errorMismatch(yylineno);
            exit(0);
        }
        stackTable.addSymbolToProgram(id->getValue(), false, type->getType(), {});
        setValue(exp->getType());

    } else {
        // means we are working with IF ELSE Statements
        if (exp->getType() != "BOOL") {
            output::errorMismatch(yylineno);
            exit(0);
        } 
    }
}

Statement::Statement(int x, int y, Exp* exp) {
     if (exp->getType() != "BOOL") {
            output::errorMismatch(yylineno);
            exit(0);
        }
}

// Statement -> Call SC
Statement::Statement(Call * call) : Node() {};

// Statement -> ID Assign Exp SC
Statement::Statement(Node * id, Exp * exp) {
    if (!stackTable.isDefinedInProgram(id->getValue())) {
        output::errorUndef(yylineno, id->getValue());
        exit(0);
    }
    if (!LegalType((stackTable.findSymbol(id->getValue())->getType()), exp->getType())) {
        output::errorMismatch(yylineno);
        exit(0);
    }
    setValue(exp->getType());
}





