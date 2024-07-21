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
    string getName() const { return symName; }
    int getOffset() const { return symOffset; }
    bool getIsFunction() const { return isFunc; }
    string getType() const { return NameType.type; }
};

class SymbolTable {
    vector<Symbol*> symbols;
    int maxOffset;
    bool isLoop;
    string* returnedType;
    ///maybe we need to add another members later
public:
    SymbolTable(int maxOff,bool isLoop, string retType = "");
    ~SymbolTable();

    bool isDefined(const string& name);
    Symbol* findSymbol(const string& symName);
    void addSymbol(const Symbol& symbol);
};


class StackTable {

};


#endif // __SYMBOLTABLE_H_