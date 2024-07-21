#include "SymbolTeble.h"

SymbolTable::SymbolTable(int maxOff,bool isloop, string retType = "") : symbols() { 
    maxOffset = maxOff;
    isLoop = isloop;
    returnedType = new string(retType);
}

SymbolTable::~SymbolTable()
{
    delete returnedType;
    for (Symbol* symbol : symbols) 
         delete symbol; 
}

bool SymbolTable::isDefined(const string& name)
{
    for (const Symbol* symbol : symbols) {
        if (symbol->getName() == name) {
            return true;
        }
    }
    return false;
}

Symbol* SymbolTable::findSymbol(const string& name)
{   
    if(!isDefined(name)) {
        return nullptr;
    }
    for (Symbol* symbol : symbols) {
        if (symbol->getName() == name) {
            return symbol;
        }
    }
}

void SymbolTable::addSymbol(const Symbol& symbol) 
{
    if(!isDefined(symbol.getName())) {
        return;
    }
    Symbol* newSymbol = new Symbol(symbol);
    symbols.push_back(newSymbol);
    maxOffset = newSymbol->getOffset() ;
}


