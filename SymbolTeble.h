#ifndef __SYMBOLTABLE_H_
#define __SYMBOLTABLE_H_

#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

struct NameTypeInfo {
    string type;
    vector<string> names;
};


string upperCase(string str) {
    for (char& c : str) {
        c = toupper(c);
    }
    return str;
}

class Symbol {
    string symName;
    int symOffset;
    bool isFunc;
    NameTypeInfo NameType;
public:
    Symbol(const string& name, int offset, bool isfunc, const string& type, vector<string> names) : symName(name), 
                                                                        symOffset(offset) , isFunc(isfunc), NameType{type, names} {}
    ~Symbol() = default;
    ///GETTERS
    NameTypeInfo getNameType() const { return NameType; }
    string getName() const { return symName; }
    int getOffset() const { return symOffset; }
    bool getIsFunction() const { return isFunc; }
    string getType() const { return NameType.type; }
};

class SymbolTable {
    
    int currentOffset;
    bool isLoop;
    string* returnedType;
    ///maybe we need to add another members later
public:
    vector<Symbol*> symbols;
    SymbolTable(int maxOff,bool isLoop, string retType = "");
    ~SymbolTable();
    int getOffset() const { return currentOffset; }
    bool getIsLoop() const { return isLoop; }
    string getReturnedType() const { return *returnedType; }
    bool isDefinedInTable(const string& name);
    Symbol* findSymbol(const string& symName);
    void addSymbol(const Symbol& symbol);
};


class StackTable {
    vector<SymbolTable*> scopes;
    vector<int> offsets;
public:
    StackTable() {
        SymbolTable* program = new SymbolTable(0, false);
        scopes.push_back(program);
        offsets.push_back(0);
        program->addSymbol(Symbol("print", 0, true, "void", {"string"}));
        program->addSymbol(Symbol("printi", 0, true, "void", {"int"}));
        program->addSymbol(Symbol("readi", 0, true, "int", {"int"}));
    }
    ~StackTable();
    void pushScope(bool isLoop, string retType);
    void popScope();
    bool isDefinedInProgram(const string& symName);
    void addSymbolToProgram(const string& name, bool isFunc, const string& type, vector<string> names);
    //SymbolTable* getTopScope();
    Symbol* findSymbol(const string& symName);
};


#endif // __SYMBOLTABLE_H_