import csv
import random
import os
import sys

def generate_inventory(num_items):
    print(f"Generating random inventory.csv with {num_items} items...")

    # 1. Pokedex'i Okuma ve Validasyon
    # PokeID'leri listeye alıyoruz ve PokeID -> EvolutionChainID eşleştirmesini yapıyoruz
    valid_poke_ids = []
    pokedex_chain_map = {}

    with open('pokedex.csv', mode='r', encoding='utf-8') as f:
        reader = csv.DictReader(f)
        for row in reader:
            pid = row['PokeID']
            chain_id = row['EvolutionChainID']
            valid_poke_ids.append(pid)
            pokedex_chain_map[pid] = chain_id

    # 2. Tipleri (Types) Okuma
    types_map = {}
    with open('types.csv', mode='r', encoding='utf-8') as f:
        for row in csv.DictReader(f):
            types_map[row['id']] = row['identifier']

    # 3. Hareketleri (Moves) Okuma
    # MoveID -> {'name': hareket_adi, 'type': tip_adi}
    moves_info = {}
    with open('moves.csv', mode='r', encoding='utf-8') as f:
        for row in csv.DictReader(f):
            moves_info[row['id']] = {
                'name': row['identifier'],
                'type': types_map.get(row['type_id'], 'normal')
            }

    # 4. Pokemonların Öğrenebileceği Hareketleri Eşleştirme
    # PokeID -> [(move_name, move_type), (move_name, move_type), ...]
    poke_valid_moves = {}
    with open('pokemon_moves.csv', mode='r', encoding='utf-8') as f:
        for row in csv.DictReader(f):
            pid = row['pokemon_id']
            minfo = moves_info.get(row['move_id'])
            if minfo:
                if pid not in poke_valid_moves:
                    poke_valid_moves[pid] = set()
                # Set kullanarak aynı hareketin farklı yollarla öğrenilmesinden doğan kopyaları engelliyoruz
                poke_valid_moves[pid].add((minfo['name'], minfo['type']))

    # Set'leri random.choice için listeye çeviriyoruz
    for pid in poke_valid_moves:
        poke_valid_moves[pid] = list(poke_valid_moves[pid])

    # 5. Aile (Evrim Zinciri) Bazlı Şeker (Candy) Üretimi
    # Şekerler bireysel Pokemon'a değil, evrim zincirine (family) aittir.
    # Örn: Pichu, Pikachu ve Raichu aynı şeker havuzunu paylaşır.
    family_candy = {}
    for chain_id in set(pokedex_chain_map.values()):
        # Her aile için 0 ile 300 arası rastgele bir şeker miktarı belirliyoruz
        family_candy[chain_id] = random.randint(0, 300)

    # 6. Rastgele Envanter Üretimi
    current_year = 2026 # İçinde bulunduğumuz yıl referansı

    with open('inventory.csv', mode='w', encoding='utf-8', newline='') as fout:
        writer = csv.writer(fout)

        # C Struct'ı ile birebir uyuşan devasa CSV başlığı
        writer.writerow([
            'InstanceID', 'PokeID', 'CP', 'HP',
            'Move1', 'Move1Type', 'Move2', 'Move2Type',
            'IsShiny', 'IsShadow',
            'IV_Attack', 'IV_Defense', 'IV_HP',
            'CatchYear', 'CatchDaysAgo', 'CandyCount'
        ])

        for instance_id in range(1, num_items + 1):
            poke_id = random.choice(valid_poke_ids)
            chain_id = pokedex_chain_map[poke_id]

            # Temel Statlar
            cp = random.randint(10, 4000)
            hp = random.randint(10, 300)
            is_shiny = 1 if random.random() < 0.05 else 0   # %5 Parlak ihtimali
            is_shadow = 1 if random.random() < 0.10 else 0  # %10 Gölge ihtimali

            # IV Üretimi (0-15 arası)
            iv_atk = random.randint(0, 15)
            iv_def = random.randint(0, 15)
            iv_hp = random.randint(0, 15)

            # Yakalanma Zamanı Üretimi
            days_ago = random.randint(0, 3800)
            catch_year = current_year - (days_ago // 365)

            # Şeker Miktarı (Evrim zinciri havuzundan çekilir!)
            candy_count = family_candy.get(chain_id, 0)

            # Hareket Üretimi
            possible_moves = poke_valid_moves.get(poke_id, [("struggle", "normal")])
            if len(possible_moves) >= 2:
                selected = random.sample(possible_moves, 2)
                m1, m1_t = selected[0]
                m2, m2_t = selected[1]
            else:
                m1, m1_t = possible_moves[0]
                m2, m2_t = "None", "None"

            writer.writerow([
                instance_id, poke_id, cp, hp,
                m1, m1_t, m2, m2_t,
                is_shiny, is_shadow,
                iv_atk, iv_def, iv_hp,
                catch_year, days_ago, candy_count
            ])

    print(f"Bitti! 'inventory.csv' oluşturuldu.")

if __name__ == "__main__":
    # Gerekli dosyaların varlığını kontrol et
    required_files = ['pokedex.csv', 'types.csv', 'moves.csv', 'pokemon_moves.csv']
    for req in required_files:
        if not os.path.exists(req):
            print(f"Hata: Eksik dosya: '{req}'. pokedex.csv ve diğer csv dosyalarının varlığını doğrulayınız.")
            sys.exit(1)

    # Komut satırından sayı alınabilir (VPL'de kolaylık sağlar)
    num_items = 250 # Varsayılan değer
    if len(sys.argv) > 1 and sys.argv[1].isdigit():
        num_items = int(sys.argv[1])
        
    # --- SEED KONTROLÜ ---
    if len(sys.argv) > 2 and sys.argv[2].isdigit():
        seed_value = int(sys.argv[2])
        random.seed(seed_value)

    generate_inventory(num_items)