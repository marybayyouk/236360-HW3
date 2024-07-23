#include "SymbolTable.h"
#include "hw3_output.hpp"


string upperCase(string str) {
    for (char& c : str) {
        c = toupper(c);
    }
    return str;
}

/////////////////////////////////////////////////SymbolTable//////////////////////////////////////////////////////////
SymbolTable::SymbolTable(int maxOff,bool isloop, string retType = "") : symbols() { 
    currentOffset = maxOff;
    isLoop = isloop;
    returnedType = new string(retType);
}

SymbolTable::~SymbolTable() {
    delete returnedType;
    for (Symbol* symbol : symbols) 
         delete symbol; 
}

bool SymbolTable::isDefinedInTable(const string& name) {
    for (const Symbol* symbol : symbols) {
        if (symbol->getName() == name) {
            return true;
        }
    }
    return false;
}

Symbol* SymbolTable::findSymbol(const string& name){   
    if(!isDefinedInTable(name)) {
        return nullptr;
    }
    for (Symbol* symbol : symbols) {
        if (symbol->getName() == name) {
            return symbol;
        }
    }
}

void SymbolTable::addSymbol(const Symbol& symbol) {
    if(!isDefinedInTable(symbol.getName())) {
        return;
    }
    Symbol* newSymbol = new Symbol(symbol);
    symbols.push_back(newSymbol);
    currentOffset = newSymbol->getOffset() ;
}


/////////////////////////////////////////////////StackTable//////////////////////////////////////////////////////////
StackTable::StackTable() {
    SymbolTable* program = new SymbolTable(0, false);
    scopes.push_back(program);
    offsets.push_back(0);
    program->addSymbol(Symbol("print", 0, true, "void", {"string"}));
    program->addSymbol(Symbol("printi", 0, true, "void", {"int"}));
    program->addSymbol(Symbol("readi", 0, true, "int", {"int"}));
}

void StackTable::pushScope(bool isLoop, string retType) {
    SymbolTable* newScope = new SymbolTable(offsets.back(), isLoop, retType);
    //SymbolTable* temp = scopes.back(); ///temp is the current scope
    if (scopes.size() > 0)
        offsets.push_back(scopes.back()->getOffset()); 
}

void StackTable::popScope() {
    SymbolTable* temp = scopes.back();
    output::endScope();
    for (Symbol* symbol : temp->symbols) {
        string name = symbol->getName();
        int offset = symbol->getOffset();
        if (!symbol->getIsFunction()) {
            output::printID(name, offset, upperCase(symbol->getType()));
        }
        else {
            vector<string> args = symbol->getNameType().names;
            for(string& arg : args) {
                arg = upperCase(arg);
            }
            string funcType = output::makeFunctionType(upperCase(symbol->getType()), args);
            output::printID(name, 0, funcType);
        }
    }
    delete temp;
    if (scopes.size() > 0) {
        scopes.pop_back();
    }
    if (offsets.size() > 0) {
        offsets.pop_back();
    }
}

bool StackTable::isDefinedInProgram(const string& name) {
    for (SymbolTable* scope : scopes) {
        if (scope->isDefinedInTable(name)) {
            return true;
        }
    }
    return false;
}

Symbol* StackTable::findSymbol(const string& name) {
    for (SymbolTable* scope : scopes) {
        Symbol* symbol = scope->findSymbol(name);
        if (symbol != nullptr) {
            return symbol;
        }
    }
    return nullptr;
}

void StackTable::addSymbolToProgram(const string& name, bool isFunc, const string& type, vector<string> names) {
    int newOffset = 0;
    if(!isFunc) {
        newOffset = offsets.back();
        offsets.push_back(newOffset + 1); ////do we really need to push_back?
    }
    Symbol newSymbol(name, newOffset, isFunc, type, names);
    scopes.back()->addSymbol(newSymbol);
}

SymbolTable* StackTable::getScope(){
    return scopes.back();
}
