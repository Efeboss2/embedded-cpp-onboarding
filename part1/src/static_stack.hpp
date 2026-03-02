#pragma once
#include <array>
#include <cstddef>
#include <optional>

class StaticStack {
public:
    static constexpr std::size_t CAPACITY = 16;

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