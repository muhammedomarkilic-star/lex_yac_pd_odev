%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pogo_types.h"

FilterNode* create_logic_node(NodeType type, FilterNode *left, FilterNode *right);
FilterNode* create_range_node(NodeType type, int min_val, int max_val);
FilterNode* create_string_node(NodeType type, const char *str);
FilterNode* create_int_node(NodeType type, int val);
// Bison'a lexer fonksiyonu hakkında bilgi ver
int yylex(void);
void yyerror(const char *s);




// main.c'nin beklediği global AST root değişkenini tanımla
FilterNode *ast_root = NULL;
%}

// Daha sonra derleme uyarılarını önlemek için en az bir union alanına ihtiyacımız var
%union {
    int intig;
    char *stringg;
    FilterNode *node;
}
%token <intig> sayi

%token <stringg> string
%type <node> islemler
%token <stringg> boolkey
%token ve 
%token <intig> yildiz
//işlemler
%token  veya 

%token degil
%token kucuktur 
%token buyuktur 
%token tire
//işlem önceliği en alttaki daha öncelikli
%left veya
%left ve
%right degil //ilk sağa bak sol zaten boş olması gerek 
%%

// En az bir kural içeren en basit gramer kuralı: boş bir sorgu.
// Bu kural sadece ast_root'un NULL olduğundan emin olur.
// Gramer kurallarını burada tanımlayın. Şimdilik sadece boş bir sorgu var.
query:
    islemler { ast_root = $1; }
    /* boş */   { ast_root = NULL; }
    | error     { ast_root = NULL; }
    ;
islemler:
    islemler veya islemler { $$ = create_logic_node(NODE_OR, $1, $3); }
    | islemler ve islemler { $$ = create_logic_node(NODE_AND, $1, $3); }
    | degil islemler { $$ = create_logic_node(NODE_NOT, $2, NULL); }
    | kucuktur string { $$ = create_string_node(NODE_WEAK_TO, $2); free($2); }
    | buyuktur string { $$ = create_string_node(NODE_STRONG_AGAINST, $2); free($2); }
    | yildiz { $$ = create_int_node(NODE_IV_STAR, $1); }
    | string { $$ = create_string_node(NODE_STRING_MATCH, $1); free($1); }
    | boolkey { $$ = create_string_node(NODE_BOOL_FLAG, $1); free($1); }
    | string sayi tire sayi {
        if (strcmp($1, "cp") == 0)        $$ = create_range_node(NODE_CP_RANGE, $2, $4);
        else if (strcmp($1, "hp") == 0)   $$ = create_range_node(NODE_HP_RANGE, $2, $4);
        else if (strcmp($1, "age") == 0)  $$ = create_range_node(NODE_AGE_RANGE, $2, $4);
        else if (strcmp($1, "year") == 0) $$ = create_range_node(NODE_YEAR_RANGE, $2, $4);
        else if (strcmp($1, "id") == 0)   $$ = create_range_node(NODE_ID_RANGE, $2, $4);
        free($1);
    }
    ;

%%

// Gerekli hata işleme fonksiyonu
void yyerror(const char *s) {
    fprintf(stderr, "Parser Hatası: %s\n", s);
}
