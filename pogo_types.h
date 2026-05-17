#ifndef POGO_TYPES_H
#define POGO_TYPES_H

#include <stdbool.h>
#include <stdint.h>

#define MAX_STR_LEN 32

/* =========================================================================
 * 1. VERİ YAPILARI(CSV SATIRLARINI TUTAN YAPILAR)
 * ========================================================================= */

// 'pokedex.csv' dosyasındaki bir satırı temsil eder(Ana referans)
// Pokemon türü hakkındaki temel bilgileri içerir.
typedef struct {
    int poke_id;            // Pokemon ID'si (örneğin: 1, 2, 3...)
    char name[32];          // Pokemon adı (örneğin: "Bulbasaur")
    char type1[32];         // Birincil tipi (örneğin: "Grass")
    char type2[32];         // İkincil tipi (örneğin: "Poison", boş ise "")
    int evolution_chain_id; // Evrim zinciri ID'si (örneğin: 1, 2, 3... aynı zincirdeki Pokemonlar aynı ID'ye sahip)
    bool is_legendary;      // Efsanevi mi?
    bool is_mythical;       // Mitolojik mi?
    int candy_to_evolve;    // Evrim için gerekli şeker sayısı
} PokemonBase;

// 'inventory.csv' dosyasındaki bir satırı temsil eder
// Envanterdeki bir Pokemonun özgül bilgilerini içerir.
typedef struct {
    int instance_id;        // Envanterdeki Pokemonun ID'si (örneğin: 1001, 1002...)
    int poke_id;            // Hangi PokemonBase'e ait olduğunu gösteren poke_id (örneğin: 1, 2, 3...)
    int cp;                 // Combat Power (Savaş Gücü)
    int hp;                 // Hit Points (Can Puanı)
    char move1[32];         // Birinci hareketin adı (örneğin: "Vine Whip")
    char move1_type[32];    // Birinci hareketin tipi (örneğin: "Grass")
    char move2[32];         // İkinci hareketin adı (örneğin: "Sludge Bomb", boş ise "")
    char move2_type[32];    // İkinci hareketin tipi (örneğin: "Poison", boş ise "")
    bool is_shiny;          // Parlak mı?
    bool is_shadow;         // Gölge mi?
    int iv_attack;          // 0-15
    int iv_defense;         // 0-15
    int iv_hp;              // 0-15
    int catch_year;         // Yakalandığı yıl (örneğin: 2020)
    int catch_days_ago;     // Yakalanalı kaç gün oldu (örneğin: 30)
    int candy_count;        // Sahip olunan şeker sayısı (örneğin: 25), evrimleştirme için kullanılır
} PokemonInstance;

// AST(Soyut Sözdizim Ağacı) düğüm türlerini tanımlayan enum
typedef enum {
    NODE_AND, NODE_OR, NODE_NOT,                    // Mantıksal operatörler
    NODE_CP_RANGE, NODE_HP_RANGE,                   // Dövüş gücü ve can puanı aralıkları
    NODE_AGE_RANGE, NODE_YEAR_RANGE, NODE_ID_RANGE, // Tarih, Yıl ve ID aralıkları
    NODE_IV_STAR,                                   // IV yıldızı, 0, 1, 2, 3, 4
    NODE_STRING_MATCH,                              // Tür, ad, hareket ve hareket tipi eşleştirmeleri
    NODE_WEAK_TO,                                   // Zayıf olduğu türler <tip
    NODE_STRONG_AGAINST,                            // Güçlü olduğu türler
    NODE_BOOL_FLAG                                  // Parlak, gölge ve evrimleşebilir gibi boolean özellikler
} NodeType;

// Bison/Yacc tarafından oluşturulan AST'nin temel düğümü
typedef struct FilterNode {
    NodeType type; // Düğümün türü (örneğin: NODE_AND, NODE_CP_RANGE, NODE_STRING_MATCH, vb.)

    // Mantıksal düğümler(AND, OR, NOT) için sol ve sağ alt düğümlerini temsil eder
    // NOT düğümleri için sadece 'left' kullanılır, 'right' NULL olur
    struct FilterNode *left;    // Sol çocuk düğüm (Yaprak düğümler için NULL)
    struct FilterNode *right;   // Sağ çocuk düğüm (Yaprak düğümler ve NOT düğümleri için NULL)

    // --- Sadece yaprak düğümler için kullanılır (Koşullar) ---
    // Koşul düğümleri için gerekli veriler. Hangi türde koşul olduğuna bağlı olarak farklı alanlar kullanılır.
    int min_val;                // Aralıkların minimum değeri (örneğin: CP >= min_val)
    int max_val;                // Aralıkların maksimum değeri (örneğin: CP <= max_val), eğer max_val == INT_MAX ise üst sınır yok demektir
    char string_val[MAX_STR_LEN]; // Ad ve tür eşleştirmeleri için kullanılan string değer (örneğin: type1 == "Fire")
    bool bool_val;              // shiny ve shadow gibi boolean özellikler için kullanılan değer (örneğin: is_shiny == true)
} FilterNode;


/* =========================================================================
 * 3. GEREKLİ FONKSİYON PROTOTİPLERİ
 * ========================================================================= */
// Öğrencilerin bu fonksiyonları ast.c / main.c dosyalarında uygulamaları gerekmektedir
// böylece Oracle ve test scriptleri bu fonksiyonlarla iletişim kurabilir.

/**
 * Verilen tek bir Pokemon örneğinin, verilen AST'ye göre arama kriterlerini karşılayıp karşılamadığını değerlendirir.
 * @param inst Spesifik bir pokemon örneğine işaretçi (PokemonInstance yapısı)
 * @param base İlgili pokemonun pokedex kaydına işaretçi (PokemonBase yapısı)
 * @param root Sorgu ifadesinin soyut sözdizim ağacının kök düğümüne işaretçi (FilterNode yapısı)
 * @return Eğer pokemon verilen kriterlere uyuyorsa true, aksi halde false döner.
 */
bool evaluate_pokemon(const PokemonInstance *inst, const PokemonBase *base, const FilterNode *root);

/**
 * yacc eylemlerinde kolayca AST düğümleri oluşturmak için yardımcı fonksiyonlar.
 * Bu fonksiyonlar, verilen tür ve ilgili verilerle yeni bir FilterNode oluşturur ve döner.
 */
FilterNode* create_logic_node(NodeType type, FilterNode *left, FilterNode *right);
FilterNode* create_range_node(NodeType type, int min_val, int max_val);
FilterNode* create_string_node(NodeType type, const char *str);
FilterNode* create_bool_node(NodeType type, bool val);
FilterNode* create_int_node(NodeType type, int val);

/**
 * AST'nin belleğini serbest bırakır. Bu, AST'nin tüm düğümlerini dolaşarak malloc ile ayrılan belleği temizler.
 */
void free_ast(FilterNode *root);

#endif // POGO_TYPES_H
