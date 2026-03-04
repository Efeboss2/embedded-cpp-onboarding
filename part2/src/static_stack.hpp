#pragma once
#include <array>
#include <cstddef>
#include <optional>
#include <cstdint> // int32_t gibi sabit boyutlu tipler için gerekliymiş

struct Sensor {  //sensör verilerini temsil etmesi için bir struct oluşturduk
    int32_t id;
    float value;
    const char* name;  // ← Bu pointer'a dikkat, Exercise 3.2'de çok işimize yarayacak!
};

class StaticStack {
public:
    static constexpr std::size_t CAPACITY = 16;
    
    // 1. By Value (Değer kopyası döner)
    std::optional<int> top_element_value() const {
        if (is_empty()) return std::nullopt;
        return _data[_top - 1]; 
        // Yorum: Asla dangle olmaz. Verinin tam bir kopyasını oluşturup dışarı verir. 
        // En güvenli (safest default) yöntem budur çünkü bellek yönetimi riski sıfırdır.
    }

    // 2. Raw Pointer (Bellek adresi döner)
    const int* top_element_pointer() const {
        if (is_empty()) return nullptr;
        return &_data[_top - 1];
        // Yorum: Eğer bu pointer'ı alan kod parçası, pointer'ı saklar ve 
        // bu sırada StaticStack objesi silinirse (destroyed), bu pointer dangle olur.
    }

    // 3. Const Reference (Değiştirilemez takma ad döner)
    const int& top_element_reference() const {
        // Not: Stack boşken referans dönmek çok tehlikelidir, genelde exception atılır.
        // Basitlik adına boş olmadığını varsayıyoruz.
        return _data[_top - 1];
        // Yorum: Tıpkı pointer gibi, eğer StaticStack objesinin yaşam döngüsü (lifetime) biterse, 
        // bu referans da anında dangle (boşlukta sallanan) duruma düşer ve UB yaratır.
    }



    // Rafa (yığına) eleman ekleme
    bool push(int value) {
        if (is_full()) {
            return false; // Raf doluysa ekleme yapma, başarısız dön
        }
        _data[_top] = value; // Sayıyı sıradaki boş kutuya koy
        _top++;              // Parmağını (işaretçiyi) bir sonraki kutuya kaydır
        return true;         // Başarıyla eklendi
    }

    // Raftan (yığından) en üstteki elemanı alma ve silme
    std::optional<int> pop() {
        if (is_empty()) {
            return std::nullopt; // Raf boşsa, "boş kutu" gönder (hata veya çökme yok)
        }
        _top--;              // Parmağını bir geri çek (en son eklenen elemana gel)
        return _data[_top];  // O elemanı okuyup geri döndür
    }

    // Raftaki en üstteki elemana sadece bakma (silmeden)
    std::optional<int> peek() const {
        if (is_empty()) {
            return std::nullopt; // Raf boşsa "boş kutu" gönder
        }
        return _data[_top - 1];  // Sadece bir alt kutuya bak ama parmağının yerini değiştirme
    }

    // Raf boş mu kontrolü
    bool is_empty() const {
        return _top == 0;
    }

    // Raf dolu mu kontrolü
    bool is_full() const {
        return _top == CAPACITY;
    }

    // Rafta kaç eleman var
    std::size_t size() const {
        return _top;
    }

private:
    std::array<int, CAPACITY> _data{}; // 16 elemanlı fiziksel hafızamız
    std::size_t _top{0};               // Bir sonraki boş kutuyu gösteren işaretçimiz
};