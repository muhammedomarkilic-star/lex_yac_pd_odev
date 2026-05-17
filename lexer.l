%{
#include <stdio.h>
#include "pogo_types.h"
#include "parser.tab.h" // "bison -d" ile oluşturulur
%}

/* Bu seçenek, bazı ortamlarda flex kütüphanesini bağlama ihtiyacını önler. -lfl olmadan */
%option noyywrap

%%

[ \t\n\r]+    { /* Beyaz boşluk ve satır sonu göz ardı et */ }
.             { /* Şimdilik herhangi bir karakteri göz ardı et */ }

%%
