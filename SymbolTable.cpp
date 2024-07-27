#include "SymbolTable.hpp"

string upperCase(string str) {
    for (char& c : str) {
        c = toupper(c);
    }
    return str;
}

/////////////////////////////////////////////////SymbolTable//////////////////////////////////////////////////////////
SymbolTable::SymbolTable(int maxOff,bool isloop) : symbols() { 
    currentOffset = maxOff;
    isLoop = isloop;
}

SymbolTable::~SymbolTable() {
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
    return nullptr;
}

void SymbolTable::addSymbol(Symbol* symbol) {
    if(isDefinedInTable(symbol->getName())) {
        return;
    }
    symbols.push_back(symbol);
    currentOffset = symbol->getOffset() ;
}

/////////////////////////////////////////////////StackTable//////////////////////////////////////////////////////////
StackTable::StackTable() {
    //SymbolTable* program = new SymbolTable(0, false);
    //scopes.push_back(program);
    offsets.push_back(0);
    pushScope(false);
    Symbol * printS = new Symbol("print", 0, true, "STRING", {"STRING"});
    Symbol * printIs = new Symbol("printi", 0, true, "INT", {"INT"});
    Symbol * readIs = new Symbol("readi", 0, true, "INT", {"INT"});
    (scopes.back())->addSymbol(printS);
    (scopes.back())->addSymbol(printIs);
    (scopes.back())->addSymbol(readIs);
}

StackTable::~StackTable() {
    for(SymbolTable* scope : scopes) {
        delete scope;
    }
}

void StackTable::pushScope(bool isLoop) {
    //SymbolTable* toAdd = new SymbolTable(offsets.back(), isLoop);
    scopes.push_back(new SymbolTable(offsets.back(), isLoop));
    if (scopes.size() > 0)
        offsets.push_back(scopes.back()->getOffset()); 
}

string StackTable::setFunctionType(string funcName) {
    if(funcName == "print") {
        return output::makeFunctionType("STRING", "VOID");
    }
    else if(funcName == "printi") {
        return output::makeFunctionType("INT", "VOID");
    }
    return output::makeFunctionType("INT", "INT");
}

void StackTable::popScope() {
    SymbolTable* temp = scopes.back();
    output::endScope();
    for (Symbol* symbol : temp->symbols) {
        string name = symbol->getName();
        int offs = symbol->getOffset();
        string type = upperCase(symbol->getType());
        if(symbol->getIsFunction()) {
            type = setFunctionType(name);
        }
        output::printID(name, offs, type);
    }
    scopes.pop_back();
    offsets.pop_back();
    // if (!scopes.empty()) {
    //     scopes.pop_back();
    // }
    // if (!offsets.empty()) {
    //     offsets.pop_back();
    // }
    delete temp;
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
    SymbolTable* toRecover = scopes.back();
    offsets.back() += 1;
    if(!isFunc) {
        //newOffset = offsets.back();
        //offsets.push_back(newOffset + 1); 
    }
    Symbol * newSymbol = new Symbol(name, (offsets.back() - 1), isFunc, type, names);
    toRecover->addSymbol(newSymbol);
}

SymbolTable* StackTable::getScope(){
    return scopes.back();
}