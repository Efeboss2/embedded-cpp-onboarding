#pragma once
#include <cstdint>
#include <cstddef>
#include <algorithm> // std::copy için ( by value döndürme muhabbeti)

struct RawBuffer {
    uint8_t* data;
    std::size_t size;
    
    // Normal Constructor
    explicit RawBuffer(std::size_t n) : data(new uint8_t[n]), size(n) {}
    
    // 1. Destructor
    ~RawBuffer() {
        delete[] data;
    }
    
    // 2. Copy Constructor
    RawBuffer(const RawBuffer& other) : data(new uint8_t[other.size]), size(other.size) {
        std::copy(other.data, other.data + other.size, data);
    }
    
    // 3. Copy Assignment Operator
    RawBuffer& operator=(const RawBuffer& other) {
        if (this == &other) return *this; 
        delete[] data; 
        size = other.size;
        data = new uint8_t[size];
        std::copy(other.data, other.data + size, data);
        return *this;
    }
    
    // 4. Move Constructor
    RawBuffer(RawBuffer&& other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr; 
        other.size = 0;
    }
    
    // 5. Move Assignment Operator
    RawBuffer& operator=(RawBuffer&& other) noexcept {
        if (this == &other) return *this;
        delete[] data; 
        data = other.data; 
        size = other.size;
        other.data = nullptr; 
        other.size = 0;
        return *this;
    }
};