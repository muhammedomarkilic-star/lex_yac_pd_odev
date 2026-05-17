%{
#include <stdio.h>
#include <stdlib.h>
#include "pogo_types.h"

// Bison'a lexer fonksiyonu hakkında bilgi ver
int yylex(void);
void yyerror(const char *s);

// main.c'nin beklediği global AST root değişkenini tanımla
FilterNode *ast_root = NULL;
%}

// Daha sonra derleme uyarılarını önlemek için en az bir union alanına ihtiyacımız var
%union {
    int dummy;
}

%%

// En az bir kural içeren en basit gramer kuralı: boş bir sorgu.
// Bu kural sadece ast_root'un NULL olduğundan emin olur.
// Gramer kurallarını burada tanımlayın. Şimdilik sadece boş bir sorgu var.
query:
    /* boş */   { ast_root = NULL; }
    | error     { ast_root = NULL; }
    ;

%%

// Gerekli hata işleme fonksiyonu
void yyerror(const char *s) {
    fprintf(stderr, "Parser Hatası: %s\n", s);
}
