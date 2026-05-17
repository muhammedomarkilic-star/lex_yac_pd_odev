#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include "pogo_types.h"

// Tüm Pokedex verisini tutan global dizi ve eleman sayısı
extern PokemonBase *g_pokedex;
extern int g_pokedex_count;

// Oyuncunun envanterini tutan global dizi ve eleman sayısı
extern PokemonInstance *g_inventory;
extern int g_inventory_count;

// Tip etkinlik tablosunu tutan yapı
typedef struct {
    char attacker[32]; // Saldıran tip
    char defender[32]; // Savunan tip
    int multiplier; // Hasar çarpanı (örneğin: 100 = normal, 200 = süper etkili, 50 = az etkili)
} TypeEffectiveness;

// Tip etkileşim tablosunu tutan global dizi ve eleman sayısı
extern TypeEffectiveness *g_type_chart;
extern int g_type_chart_count;

// CSV dosyalarını okuyup global dizileri dolduran ana fonksiyon
// Dönüş: Başarılı ise 1, hata varsa 0
int load_all_data(const char *pokedex_file, const char *inventory_file, const char *type_chart_file);

// Belleği temizleyen fonksiyon
void free_all_data();

// Yardımcı Fonksiyon: Bir PokeID'ye karşılık gelen Pokedex bilgisini hızlıca bulur
PokemonBase* get_base_pokemon(int poke_id);

// Yardımcı Fonksiyon: Tip etkileşimini sorgular (100: normal, 200: super, 50: not very)
int get_type_multiplier(const char *attacker_type, const char *defender_type);

#endif
