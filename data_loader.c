#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_loader.h"

// Global değişkenlerin tanımları
PokemonBase *g_pokedex = NULL; // Pokedex verilerini tutan global dizi
int g_pokedex_count = 0;

PokemonInstance *g_inventory = NULL; // Envanter verilerini tutan global dizi
int g_inventory_count = 0;

TypeEffectiveness *g_type_chart = NULL; // Tip etkileşim tablosunu tutan global dizi
int g_type_chart_count = 0;

// --- YARDIMCI FONKSİYONLAR ---

// Bir dosyadaki satır sayısını sayar (Header hariç)
static int count_csv_lines(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) return -1;

    int lines = 0;
    char ch;
    while (!feof(file)) {
        ch = fgetc(file);
        if (ch == '\n') lines++;
    }
    fclose(file);

    // Eğer dosya boş değilse, başlık (header) satırını çıkar
    return (lines > 0) ? lines - 1 : 0;
}

// String kopyalamayı güvenli hale getiren küçük bir wrapper
static void safe_strcpy(char *dest, const char *src, size_t size) {
    if (src) {
        strncpy(dest, src, size - 1);
        dest[size - 1] = '\0';
    } else {
        dest[0] = '\0';
    }
}

// --- ANA YÜKLEME FONKSİYONLARI ---
// Verilen dosya yollarından tüm verileri yükler. Başarılıysa 1, başarısızsa 0 döner.
int load_all_data(const char *pokedex_file, const char *inventory_file, const char *type_chart_file) {
    char line[512];

    // 1. POKEDEX YÜKLEME
    g_pokedex_count = count_csv_lines(pokedex_file);
    if (g_pokedex_count <= 0) {
        fprintf(stderr, "HATA: %s bulunamadi veya bos.\n", pokedex_file);
        return 0;
    }

    g_pokedex = (PokemonBase *)malloc(sizeof(PokemonBase) * g_pokedex_count);
    FILE *f_poke = fopen(pokedex_file, "r");
    fgets(line, sizeof(line), f_poke); // Başlığı atla

    int i = 0;
    while (fgets(line, sizeof(line), f_poke) && i < g_pokedex_count) {
        line[strcspn(line, "\r\n")] = 0; // Satır sonu karakterlerini temizle

        // PokeID, Name, Type1, Type2, EvolutionChainID, IsLegendary, IsMythical, CandyToEvolve
        char *tok = strtok(line, ",");
        if(!tok) continue;
        g_pokedex[i].poke_id = atoi(tok);

        tok = strtok(NULL, ","); safe_strcpy(g_pokedex[i].name, tok, 32);
        tok = strtok(NULL, ","); safe_strcpy(g_pokedex[i].type1, tok, 32);
        tok = strtok(NULL, ","); safe_strcpy(g_pokedex[i].type2, tok, 32);

        tok = strtok(NULL, ","); g_pokedex[i].evolution_chain_id = tok ? atoi(tok) : 0;
        tok = strtok(NULL, ","); g_pokedex[i].is_legendary = tok ? atoi(tok) : 0;
        tok = strtok(NULL, ","); g_pokedex[i].is_mythical = tok ? atoi(tok) : 0;
        tok = strtok(NULL, ","); g_pokedex[i].candy_to_evolve = tok ? atoi(tok) : 0;
        i++;
    }
    fclose(f_poke);

    // 2. ENVANTER YÜKLEME
    g_inventory_count = count_csv_lines(inventory_file);
    if (g_inventory_count <= 0) {
        fprintf(stderr, "HATA: %s bulunamadi veya bos.\n", inventory_file);
        return 0;
    }

    g_inventory = (PokemonInstance *)malloc(sizeof(PokemonInstance) * g_inventory_count);
    FILE *f_inv = fopen(inventory_file, "r");
    fgets(line, sizeof(line), f_inv); // Başlığı atla

    i = 0;
    while (fgets(line, sizeof(line), f_inv) && i < g_inventory_count) {
        line[strcspn(line, "\r\n")] = 0;

        // InstanceID, PokeID, CP, HP, Move1, Move1Type, Move2, Move2Type, IsShiny, IsShadow, IV_Attack, IV_Defense, IV_HP, CatchYear, CatchDaysAgo, CandyCount
        char *tok = strtok(line, ",");
        if(!tok) continue;
        g_inventory[i].instance_id = atoi(tok);
        tok = strtok(NULL, ","); g_inventory[i].poke_id = tok ? atoi(tok) : 0;
        tok = strtok(NULL, ","); g_inventory[i].cp = tok ? atoi(tok) : 0;
        tok = strtok(NULL, ","); g_inventory[i].hp = tok ? atoi(tok) : 0;

        tok = strtok(NULL, ","); safe_strcpy(g_inventory[i].move1, tok, 32);
        tok = strtok(NULL, ","); safe_strcpy(g_inventory[i].move1_type, tok, 32);
        tok = strtok(NULL, ","); safe_strcpy(g_inventory[i].move2, tok, 32);
        tok = strtok(NULL, ","); safe_strcpy(g_inventory[i].move2_type, tok, 32);

        tok = strtok(NULL, ","); g_inventory[i].is_shiny = tok ? atoi(tok) : 0;
        tok = strtok(NULL, ","); g_inventory[i].is_shadow = tok ? atoi(tok) : 0;

        tok = strtok(NULL, ","); g_inventory[i].iv_attack = tok ? atoi(tok) : 0;
        tok = strtok(NULL, ","); g_inventory[i].iv_defense = tok ? atoi(tok) : 0;
        tok = strtok(NULL, ","); g_inventory[i].iv_hp = tok ? atoi(tok) : 0;

        tok = strtok(NULL, ","); g_inventory[i].catch_year = tok ? atoi(tok) : 0;
        tok = strtok(NULL, ","); g_inventory[i].catch_days_ago = tok ? atoi(tok) : 0;
        tok = strtok(NULL, ","); g_inventory[i].candy_count = tok ? atoi(tok) : 0;
        i++;
    }
    fclose(f_inv);

    // 3. TİP ETKİLEŞİM TABLOSU YÜKLEME
    g_type_chart_count = count_csv_lines(type_chart_file);
    if (g_type_chart_count > 0) {
        g_type_chart = (TypeEffectiveness *)malloc(sizeof(TypeEffectiveness) * g_type_chart_count);
        FILE *f_type = fopen(type_chart_file, "r");
        fgets(line, sizeof(line), f_type); // Başlığı atla

        i = 0;
        while (fgets(line, sizeof(line), f_type) && i < g_type_chart_count) {
            line[strcspn(line, "\r\n")] = 0;

            // AttackerType, DefenderType, DamageFactor
            char *tok = strtok(line, ",");
            if(!tok) continue;
            safe_strcpy(g_type_chart[i].attacker, tok, 32);

            tok = strtok(NULL, ","); safe_strcpy(g_type_chart[i].defender, tok, 32);
            tok = strtok(NULL, ","); g_type_chart[i].multiplier = tok ? atoi(tok) : 100;
            i++;
        }
        fclose(f_type);
    } else {
        fprintf(stderr, "UYARI: %s tablosu okunamadi, tip zayifligi sorgulari calismayabilir.\n", type_chart_file);
        // Tip tablosu opsiyonel kabul edilebilir, hata fırlatmıyoruz
    }

    return 1; // Başarılı
}

// --- BELLEK TEMİZLEME ---
// Yüklenen tüm veriler için ayrılan belleği serbest bırakır ve global sayaçları sıfırlar.
void free_all_data() {
    if (g_pokedex) { free(g_pokedex); g_pokedex = NULL; }
    if (g_inventory) { free(g_inventory); g_inventory = NULL; }
    if (g_type_chart) { free(g_type_chart); g_type_chart = NULL; }

    g_pokedex_count = 0;
    g_inventory_count = 0;
    g_type_chart_count = 0;
}

// --- SORGULAMA (LOOKUP) FONKSİYONLARI ---
// Verilen poke_id'ye sahip PokemonBase yapısını döner. Bulamazsa NULL döner.
PokemonBase* get_base_pokemon(int poke_id) {
    // Binary search eklenebilir ancak maksimum ~1000 Pokemon olduğu için
    // lineer arama (linear search) bu proje için yeterince hızlıdır.
    for (int i = 0; i < g_pokedex_count; i++) {
        if (g_pokedex[i].poke_id == poke_id) {
            return &g_pokedex[i];
        }
    }
    return NULL;
}
// Verilen saldıran ve savunan tipler için hasar çarpanını döner. Bulamazsa 100 (normal hasar) döner.
int get_type_multiplier(const char *attacker_type, const char *defender_type) {
    if (!attacker_type || !defender_type) return 100;

    // String kiyaslamasi sirasinda case-sensitivity'den kacmak icin
    // Python scriptinin her seyi kucuk harf urettigini varsayiyoruz.
    for (int i = 0; i < g_type_chart_count; i++) {
        if (strcmp(g_type_chart[i].attacker, attacker_type) == 0 &&
            strcmp(g_type_chart[i].defender, defender_type) == 0) {
            return g_type_chart[i].multiplier;
        }
    }

    // Eğer tabloda tanım yoksa normal (%100) hasar varsayılır.
    return 100;
}
