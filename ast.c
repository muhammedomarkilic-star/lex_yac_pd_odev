#define _GNU_SOURCE // strcasestr için gerekli, kullanılmayacaksa silinebilir.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pogo_types.h"
#include "data_loader.h" // get_type_multiplier() için gerekli

/* =========================================================================
 * 1. AST DÜĞÜM OLUŞTURMA FONKSİYONLARI
 * Bu fonksiyonlar, parser.y eylemleriniz tarafından ağacı oluşturmak için çağrılır.
 * ========================================================================= */

// Mantıksal düğümler (AND, OR, NOT) oluşturan yardımcı fonksiyon
FilterNode* create_logic_node(NodeType type, FilterNode *left, FilterNode *right) {
    // TODO: malloc() kullanarak yeni bir FilterNode için bellek ayırın.
    // TODO: malloc() basarisiz olursa, hata mesajini yazin ve programi sonlandirin.
    // TODO: düğümün türünü, sol ve sağ işaretçilerini ayarlayın.
    // TODO: Diger alanları güvenli varsayılan değerlerle başlatın (örneğin: min_val = 0, string_val[0] = '\0', bool_val = false).

    return NULL; // Burayı uygun şekilde yeni oluşturulan düğümle değiştirin.
}

// Aralık duğümlerini oluşturan yardımcı fonksiyon
FilterNode* create_range_node(NodeType type, int min_val, int max_val) {
    // TODO: Yeni bir FilterNode için bellek ayırın(malloc)
    // TODO: Düğümün türünü, min_val ve max_val değişkenlerini ayarlayın.
    // İpucu: Bu düğüm bir yaprak olduğundan, left ve right işaretçileri NULL olmalıdır.

    return NULL; // Burayı uygun şekilde yeni oluşturulan düğümle değiştirin.
}

FilterNode* create_string_node(NodeType type, const char *str) {
    // TODO: Yeni FilterNode için bellek ayırın(malloc)
    // TODO: Düğümün türünü ayarlayın.
    // TODO: string_val dizisine 'str' parametresini güvenli bir şekilde kopyalayın (strncpy kullanın).
    // İpucu: Bu düğüm NODE_STRING_MATCH, NODE_WEAK_TO, NODE_STRONG_AGAINST ve NODE_BOOL_FLAG için kullanılır.

    return NULL; // Burayı uygun şekilde yeni oluşturulan düğümle değiştirin.
}

FilterNode* create_int_node(NodeType type, int val) {
    // TODO: Yeni bir FilterNode için bellek ayırın(malloc)
    // TODO: Düğümün türünü ayarlayın ve 'val' değerini ayarlayın
    // İpucu: Bu düğüm özellikle NODE_IV_STAR için kullanışlıdır (örneğin: 0, 1, 2, 3 veya 4 saklamak için).

    return NULL;
}

/* =========================================================================
 * 2. AST BELLEK TEMİZLEME FONKSİYONU
 * Bu fonksiyon, AST'nin tüm düğümlerini dolaşarak malloc ile ayrılan belleği
 * serbest bırakır.
 * ========================================================================= */

void free_ast(FilterNode *root) {
    // TODO: AST'yi serbest bırakmak için özyinelemeli bir fonksiyon yazın.
    // 1. Temel durum: Eğer root NULL ise, geri dönün.
    // 2. Sol çocuğu özyinelemeli olarak serbest bırakın.
    // 3. Sağ çocuğu özyinelemeli olarak serbest bırakın.
    // 4. Mevcut root düğümünü serbest bırakın.
}

/* =========================================================================
 * 3. DEĞERLENDİRME FONKSİYONU(Ana mantık burada)
 * Bu fonksiyon, tek bir Pokemonu AST'ye göre özyinelemeli olarak değerlendirir.
 * ========================================================================= */

bool evaluate_pokemon(const PokemonInstance *inst, const PokemonBase *base, const FilterNode *root) {
    // Temel durum: Ağaç boşsa her şeyle eşleşir
    // Kodun bu başlangıç hali tüm sorgularda bütün Pokemonların döndürülmesine neden olur,
    // bu yüzden bunu kaldırıp yerine gerçek değerlendirme mantığını eklemeniz gerekecek.
    if (root == NULL) {
        return true;
    }

    switch (root->type) {

        // MANTIKSAL OPERATÖRLER
        case NODE_AND:
            // TODO: Sol ve sağ çocukları özyinelemeli olarak değerlendirin ve sonuçları AND'leyin
            return false;

        case NODE_OR:
            // TODO: Sol ve sağ çocukları özyinelemeli olarak değerlendirin
            return false;

        case NODE_NOT:
            // TODO: Sol çocuğu özyinelemeli olarak değerlendirin ve sonucu TERSİNE ÇEVİRİN
            return false;

        // TAMSAYI ARALIKLARI
        case NODE_CP_RANGE:
            // TODO: inst->cp'nin root->min_val ile root->max_val(dahil) arasında olup olmadığını kontrol edin
            return false;

        case NODE_HP_RANGE:
            // TODO: inst->hp'nin ilgili aralıkta olup olmadığını kontrol edin
            return false;

        case NODE_AGE_RANGE:
            // TODO: inst->catch_days_ago'nun ilgili aralıkta olup olmadığını kontrol edin
            return false;

        case NODE_YEAR_RANGE:
            // TODO: inst->catch_year'nin ilgili aralıkta olup olmadığını kontrol edin
            return false;

        case NODE_ID_RANGE:
            // TODO: inst->poke_id'nin ilgili aralıkta olup olmadığını kontrol edin
            return false;

        // --- IV STARS (0*, 1*, 2*, 3*, 4*) ---
        case NODE_IV_STAR:
            // TODO: inst->iv_attack + inst->iv_defense + inst->iv_hp toplamını hesaplayın
            // TODO: Toplam değerinin root->min_val'a göre doğru aralığa düşüp düşmediğini kontrol edin
            // (0*: 0-22, 1*: 23-29, 2*: 30-36, 3*: 37-44, 4*: 45)
            return false;

        // STRING EŞLEŞMELERİ
        case NODE_STRING_MATCH:
            // TODO: Bu düğüm birden fazla türde arama yapabilir.
            // root->string_val değerinin aşağıdakilerden herhangi biriyle eşleşip eşleşmediğini kontrol etmelisiniz:
            // - base->type1 veya base->type2 (Tür araması, örneğin: "water")
            // - inst->move1 veya inst->move2 (Hareket araması, örneğin: "@bite")
            // - inst->move1_type veya inst->move2_type (Hareket Türü araması, örneğin: "@fighting")
            // - base->name'in bir alt dizisi (Ad araması, örneğin: "pika")
            // İpucu: Büyük/küçük harfe duyarsız karşılaştırmalar için strcasecmp() ve alt diziler için strcasestr() kullanın.
            return false;

        // TİP ETKİNLİĞİ (<type ve >type)
        case NODE_WEAK_TO:
            // TODO: Pokemonun, root->string_val'a zayıf olduğu türleri kontrol edin (örneğin: "<fire").
            // İpucu: get_type_multiplier(root->string_val, base->type1)
            // ve get_type_multiplier(root->string_val, base->type2) kullanın.
            // Sonuçları çarpın ve 100'e bölün. Eğer sonuç 100'den büyükse, true döndürün.
            return false;

        case NODE_STRONG_AGAINST:
            // TODO: Pokemonun, root->string_val'a karşı süper etkili bir hareketi olup olmadığını kontrol edin.
            // İpucu: get_type_multiplier(inst->move1_type, root->string_val)
            // VEYA get_type_multiplier(inst->move2_type, root->string_val) kullanın.
            return false;

        // BOOL BAYRAKLARI (shiny, shadow, legendary, mythical, evolve gibi)
        case NODE_BOOL_FLAG:
            // TODO: Hangi bayrağın istendiğini görmek için root->string_val'u kontrol edin.
            // Örnek: if (strcasecmp(root->string_val, "shiny") == 0) { return inst->is_shiny; }
            // Örnek: Eğer "evolve" isteniyorsa, base->candy_to_evolve > 0 VE inst->candy_count >= base->candy_to_evolve
            // olup olmadığını kontrol edin.
            return false;

        default:
            fprintf(stderr, "Bilinmeyen AST düğüm türü!\n");
            return false;
    }
}
