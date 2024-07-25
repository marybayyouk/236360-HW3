%{
    #include <stdio.h>
    #include <iostream>
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

int             { yylval= new Node(yytext); return INT; }
byte            { yylval= new Node(yytext); return BYTE; }
b               { yylval= new Node(yytext); return B; }
bool            { yylval= new Node(yytext); return BOOL; }
and             { yylval= new Node(yytext); return AND; }
or              { yylval= new Node(yytext); return OR; }
not             { yylval= new Node(yytext); return NOT; }
true            { yylval = new Node(yytext); return TRUE; }
false           { yylval = new Node(yytext); return FALSE; }
return          { yylval= new Node(yytext); return RETURN; }
if              { yylval= new Node(yytext); return IF; }
else            { yylval= new Node(yytext); return ELSE; }
while           { yylval= new Node(yytext); return WHILE; }
break           { yylval= new Node(yytext); return BREAK; }
continue        { yylval= new Node(yytext); return CONTINUE; }
(\;)            { yylval= new Node(yytext); return SC; }
(\()            { yylval= new Node(yytext); return LPAREN; }
(\))            { yylval= new Node(yytext); return RPAREN; }
(\{)            { yylval= new Node(yytext); return LBRACE; }
(\})            { yylval= new Node(yytext); return RBRACE; }
(=)             { yylval= new Node(yytext); return ASSIGN; }
>=|<=|<|>       { yylval= new Node(yytext); return RELOP;}
!=|==           { yylval= new Node(yytext); return ISEQUAL; }
\+              { yylval= new Node(yytext); return ADD; }
\-              { yylval= new Node(yytext); return SUB; }
\*              { yylval= new Node(yytext); return MULTIPLY; }
\/              { yylval= new Node(yytext); return DIVIDE; }
{ID}            { yylval= new Node(yytext); return ID;} 
0|[1-9]{digit}* { yylval = new Node(yytext); return NUM;}

\"([^\n\r\"\\]|\\[rnt"\\])+\"       { yylval = new Node(yytext); return STRING; }
\/\/[^\r\n]*\r?\n?                   ;
{whitespace}    { /* ignore whitespace */ }
.               {   
                    output::errorLex(yylineno);
                    exit(0);}
%%