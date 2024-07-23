#ifndef __SYMBOLTABLE_H_
#define __SYMBOLTABLE_H_

#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

string upperCase(string str);

struct NameTypeInfo {
    string type;
    vector<string> names;
};

class Symbol {
private:
    string symName;
    int symOffset;
    bool isFunc;
    NameTypeInfo NameType;
public:
    Symbol(const string& name, int offset, bool isfunc, const string& type, vector<string> names) : 
                                            symName(name), symOffset(offset), isFunc(isfunc), NameType{type, names} {}
    ~Symbol() = default;
    ///GETTERS
    NameTypeInfo getNameType() const { return NameType; }
    string getName() const { return symName; }
    int getOffset() const { return symOffset; }
    bool getIsFunction() const { return isFunc; }
    string getType() const { return NameType.type; }
};

class SymbolTable {
private:
    int currentOffset;
    bool isLoop;
    string* returnedType;
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
    StackTable();
    ~StackTable();
    void pushScope(bool isLoop, string retType);
    void popScope();
    bool isDefinedInProgram(const string& symName);
    void addSymbolToProgram(const string& name, bool isFunc, const string& type, vector<string> names);
    SymbolTable* getScope();
    Symbol* findSymbol(const string& symName);
};


#endif // __SYMBOLTABLE_H_