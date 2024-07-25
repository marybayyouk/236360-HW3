%{
#include "hw3_output.hpp"
#include "ProgramTypes.hpp"
#include "parser.tab.hpp"
%}

%option noyywrap
%option yylineno

digit   		  ([0-9])
letter  		  ([a-zA-Z])
whitespace	      ([\t\n\r ])
ID               {letter}({letter}|{digit})*

%%

int             { yylval.node = new Node(yytext); return INT; }
byte            { yylval.node = new Node(yytext); return BYTE; }
b               { yylval.node = new Node(yytext); return B; }
bool            { yylval.node = new Node(yytext); return BOOL; }
and             { yylval.node = new Node(yytext); return AND; }
or              { yylval.node = new Node(yytext); return OR; }
not             { yylval.node = new Node(yytext); return NOT; }
true            { yylval.node = new Node(yytext); return TRUE; }
false           { yylval.node = new Node(yytext); return FALSE; }
return          { yylval.node = new Node(yytext); return RETURN; }
if              { yylval.node = new Node(yytext); return IF; }
else            { yylval.node = new Node(yytext); return ELSE; }
while           { yylval.node = new Node(yytext); return WHILE; }
break           { yylval.node = new Node(yytext); return BREAK; }
continue        { yylval.node = new Node(yytext); return CONTINUE; }
(\;)            { yylval.node = new Node(yytext); return SC; }
(\()            { yylval.node = new Node(yytext); return LPAREN; }
(\))            { yylval.node = new Node(yytext); return RPAREN; }
(\{)            { yylval.node = new Node(yytext); return LBRACE; }
(\})            { yylval.node = new Node(yytext); return RBRACE; }
(=)             { yylval.node = new Node(yytext); return ASSIGN; }
>=|<=|<|>       { yylval.node = new Node(yytext); return RELOP;}
!=|==           { yylval.node = new Node(yytext); return ISEQUAL; }
\+              { yylval.node = new Node(yytext); return ADD; }
\-              { yylval.node = new Node(yytext); return SUB; }
\*              { yylval.node = new Node(yytext); return MULTIPLY; }
\/              { yylval.node = new Node(yytext); return DIVIDE; }
{ID}            { yylval.node = new Node(yytext); return ID;} 
0|[1-9]{digit}* { yylval.node = new Node(yytext); return NUM;}
\"([^\n\r\"\\]|\\[rnt"\\])+\"       { yylval.node = new Node(yytext); return STRING; }
\/\/[^\r\n]*\r?\n?                   ;
{whitespace}    { /* ignore whitespace */ }
.               { output::errorLex(yylineno);
                    exit(0);}
%%