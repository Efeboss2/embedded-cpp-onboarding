#include <gtest/gtest.h>
#include "static_stack.hpp"

// Senaryo 1: Yeni oluşturulan bir stack boş olmalıdır
TEST(StaticStackTest, NewlyCreatedStackIsEmpty) {
    StaticStack stack;
    EXPECT_TRUE(stack.is_empty());
    EXPECT_EQ(stack.size(), 0);
}

// Senaryo 2: Bir eleman push edildiğinde size() 1 olmalıdır
TEST(StaticStackTest, PushOneElementChangesSize) {
    StaticStack stack;
    EXPECT_TRUE(stack.push(42));
    EXPECT_EQ(stack.size(), 1);
    EXPECT_FALSE(stack.is_empty());
}

// Senaryo 3: Boş bir stack'ten pop yapmak std::nullopt döndürmelidir
TEST(StaticStackTest, PopEmptyStackReturnsNullopt) {
    StaticStack stack;
    EXPECT_EQ(stack.pop(), std::nullopt);
}

// Senaryo 4: Kapasite doluyken push yapmak false dönmeli ve sistemi bozmamalıdır
TEST(StaticStackTest, PushBeyondCapacityFailsSafely) {
    StaticStack stack;
    // Stack'i tamamen doldur (16 eleman)
    for (int i = 0; i < 16; ++i) {
        EXPECT_TRUE(stack.push(i));
    }
    EXPECT_TRUE(stack.is_full());
    EXPECT_EQ(stack.size(), 16);

    // 17. elemanı eklemeye çalış (Hata vermeli ve kapasite 16 kalmalı)
    EXPECT_FALSE(stack.push(99));
    EXPECT_EQ(stack.size(), 16); 
}

// Senaryo 5: peek() fonksiyonu elemana sadece bakmalı, onu silmemelidir
TEST(StaticStackTest, PeekDoesNotRemoveElement) {
    StaticStack stack;
    stack.push(10);
    
    auto val = stack.peek();
    EXPECT_EQ(val, 10);
    EXPECT_EQ(stack.size(), 1); // Boyut hala 1 kalmalı
}

// Senaryo 6: Sırayla push ve pop yapıldığında (LIFO) değerler doğru sırayla gelmelidir
TEST(StaticStackTest, PushPopSequenceIsCorrect) {
    StaticStack stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);

    EXPECT_EQ(stack.pop(), 3);
    EXPECT_EQ(stack.pop(), 2);
    EXPECT_EQ(stack.pop(), 1);
    EXPECT_TRUE(stack.is_empty());
}