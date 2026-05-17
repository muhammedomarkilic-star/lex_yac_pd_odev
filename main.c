#include <stdio.h>
#include <stdlib.h> // qsort icin gerekli
#include "data_loader.h"
#include "pogo_types.h"

extern FilterNode *ast_root;
extern int yyparse();

// qsort için tam sayı karşılaştırma fonksiyonu
int compare_ints(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int main(int argc, char **argv) {
    // 1. Argüman Kontrolü
    if (argc < 4) {
        fprintf(stderr, "Kullanim: ./pogo <pokedex.csv> <inventory.csv> <type_chart.csv>\n");
        return 1;
    }

    // 2. Verileri Yükle (Dosya isimlerini argümandan alıyoruz)
    if (!load_all_data(argv[1], argv[2], argv[3])) {
        return 1;
    }

    // 3. Lex/Yacc Parser'ı Çalıştır (Sorguyu standart girdiden/stdin okuyacak)
    if (yyparse() != 0) {
        fprintf(stderr, "Sorgu ayristirilirken soz dizimi hatasi olustu!\n");
        free_all_data();
        return 1;
    }

    // 4. Envanteri Filtrele ve ID'leri Topla
    int matched_ids[2000]; // Envanter sayısından büyük olmalı
    int match_count = 0;

    for (int i = 0; i < g_inventory_count; i++) {
        PokemonInstance *inst = &g_inventory[i];
        PokemonBase *base = get_base_pokemon(inst->poke_id);

        if (base && evaluate_pokemon(inst, base, ast_root)) {
            matched_ids[match_count++] = inst->instance_id;
        }
    }

    // 5. ID'leri Küçükten Büyüğe Sırala
    if (match_count > 0) {
        qsort(matched_ids, match_count, sizeof(int), compare_ints);
    }

    // 6. Sadece boşluklarla ayrılmış ID'leri yazdır (VPL karşılaştırması için)
    for (int i = 0; i < match_count; i++) {
        printf("%d ", matched_ids[i]);
    }
    printf("\n"); // Satır sonu

    // 7. Belleği Temizle
    free_all_data();
    if (ast_root != NULL) {
        free_ast(ast_root);
    }

    return 0;
}
