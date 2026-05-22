%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pogo_types.h"
#include "parser.tab.h" 
%}

%option noyywrap

%%
[ \t\n\r]+  { }
"-"         { return tire; }
"&"         { return ve; }
","         { return veya; }

"<"         { return kucuktur; }
">"         { return buyuktur; }

"!"         { return degil; }


(?i:"shadow")    { yylval.stringg = strdup(yytext); return boolkey; }
(?i:"legendary") { yylval.stringg = strdup(yytext); return boolkey; }
(?i:"mythical")  { yylval.stringg = strdup(yytext); return boolkey; }
(?i:"evolve")    { yylval.stringg = strdup(yytext); return boolkey; }
(?i:"shiny")     { yylval.stringg = strdup(yytext); return boolkey; }


[0-4]"*"    { yylval.intig = yytext[0] - '0'; return yildiz;   }


[0-9]+      { yylval.intig = atoi(yytext); return sayi;   }


@?[a-zA-Z_][a-zA-Z0-9_-]* { yylval.stringg = strdup(yytext); return string;}



.           { printf("Bilinmeyen karakter: %s\n", yytext); }

%%
