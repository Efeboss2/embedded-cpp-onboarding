#pragma once
#include "static_stack.hpp" // Stackable konseptini ve SensorReading'i kullanmak için
#include <array>
#include <cstddef>
#include <optional>
#include <utility>

template<Stackable T, std::size_t N>
class StaticRingBuffer {
public:
    // push metodu: Kuyruk doluysa en eski elemanı ezer
    void push(T&& value) {
        _data[_tail] = std::move(value); // Veriyi taşı (steal)
        _tail = (_tail + 1) % N;         // Tail indeksini ilerlet ve gerekirse başa sar
        
        if (is_full()) {
            // Eğer tam kapasiteye ulaşıldıysa, tail eski verinin üstüne yazmıştır.
            // Bu yüzden okuma noktasını (head) da bir adım ileri itmemiz gerekir.
            _head = (_head + 1) % N;
        } else {
            _count++; // Dolu değilse eleman sayısını artır
        }
    }

    std::optional<T> pop() {
        if (is_empty()) {
            return std::nullopt;
        }
        
        // En eski elemanı dışarı taşı
        std::optional<T> val = std::move(_data[_head]);
        _head = (_head + 1) % N;
        _count--;
        
        return val;
    }

    // DİKKAT: Döküman peek() metodunu by-value dönecek şekilde tanımlamış.
    // Ancak kopyalamaya kapalı (non-copyable) objelerde bu metot kullanılamaz.
    // Şablonun (template) genel geçerliliğini bozmamak adına yorum satırına alıyoruz.
    /*
    std::optional<T> peek() const {
        if (is_empty()) return std::nullopt;
        return _data[_head]; 
    }
    */

    bool is_empty() const { return _count == 0; }
    bool is_full() const { return _count == N; }
    std::size_t size() const { return _count; }

private:
    std::array<T, N> _data{};
    std::size_t _head{0};  // Okunacak (en eski) elemanın indeksi
    std::size_t _tail{0};  // Yazılacak (sıradaki boş) yerin indeksi
    std::size_t _count{0}; // Mevcut eleman sayısı
};