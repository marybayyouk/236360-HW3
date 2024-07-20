%{
#include "output.hpp"
#include "parser.tab.hpp"
%}

%option noyywrap
%option yylineno

digit   		  ([0-9])
letter  		  ([a-zA-Z])
whitespace	      ([\t\n\r ])
ID               {letter}({letter}|{digit})*

%%
int             { return INT; }
byte            { return BYTE; }
b               { return B; }
bool            { return BOOL; }
and             { return AND; }
or              { return OR; }
not             { return NOT; }
true            { return TRUE; }
false           { return FALSE; }
return          { return RETURN; }
if              { return IF; }
else            { return ELSE; }
while           { return WHILE; }
break           { return BREAK; }
continue        { return CONTINUE; }
(\;)            { return SC; }
(\()            { return LPAREN; }
(\))            { return RPAREN; }
(\{)            { return LBRACE; }
(\})            { return RBRACE; }
(=)             { return ASSIGN; }
>=|<=|<|>       {return RELOP;}
!=|==           { return ISEQUAL; }
\+              { return ADD; }
\-              { return SUB; }
\*              { return MULTIPLY; }
\/              { return DIVIDE; }
{ID}            { return ID; } 
0|[1-9]{digit}* { return NUM;}
\"([^\n\r\"\\]|\\[rnt"\\])+\"       { return STRING; }
\/\/[^\r\n]*\r?\n?                   ;
{whitespace}    { /* ignore whitespace */ }
.               { output::errorLex(yylineno);
                    exit(0);}
%%