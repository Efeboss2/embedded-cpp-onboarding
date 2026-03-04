#include <gtest/gtest.h>
#include "static_stack.hpp"
#include <iostream>

// Kasıtlı olarak HATALI yazılmış fonksiyon
//const int& get_top_value(const StaticStack& stack) {
//    std::optional<int> top = stack.peek();
//    if (top.has_value()) {
//        return top.value(); // ← Sorun tam olarak burada
//    }
//    static int sentinel = -1;
//    return sentinel;
//}
//
//TEST(Part2UBTest, DanglingReferenceExample) {
//    StaticStack stack;
//    stack.push(42);
//
//    // Kasıtlı olarak silinmiş bir bellek adresine referans alıyoruz
//    const int& bad_ref = get_top_value(stack);
//    
//    // Ekrana yazdıralım, bakalım 42 mi çıkacak yoksa garbage value (çöp değer) mu?
//    std::cout << "Top value is: " << bad_ref << std::endl;
//    
//    // UB yüzünden bu test geçebilir de kalabilir de!
//    EXPECT_EQ(bad_ref, 42); 
//}

// Düzeltilmiş fonksiyon: Reference değil, "by value" dönüyor.
std::optional<int> get_top_value(const StaticStack& stack) {
    return stack.peek(); // Direkt güvenli kopyasını döndürüyoruz 
}

TEST(Part2UBTest, SafeValueReturnExample) {
    StaticStack stack;
    stack.push(42);

    // Artık silinmiş bir adres değil, gerçek bir kopyaya (value) sahibiz
    std::optional<int> safe_val = get_top_value(stack);
    
    // Değerin başarıyla geldiğini ve tam olarak 42 olduğunu doğruluyoruz
    EXPECT_TRUE(safe_val.has_value());
    EXPECT_EQ(safe_val.value(), 42); 
}// Bu test, artık Undefined Behavior içermiyor ve her zaman geçecektir.


// **********************************
// BU DURUMDA KODU YAZARKEN LOCAL VARİABLDEN ALDIN DATAYI SİLMEK YERİNE KOPYALAYIP UB DÖNDÜRMEMEK İÇİN BY VALUE DÖNDÜRMEK LAZIM
// **********************************
//      KENDİME NOT
//Yapay Zekaya sordum 
//1. const int& ve std::optional<int> Arasındaki Fark Nedir?
//Bu iki kavram C++'ta tamamen farklı amaçlara hizmet eder.

//std::optional<int> (İçinde Değer Olabilen Kutu):
//C++ statically typed (katı tipli) bir dildir. Bir fonksiyon int dönecek diyorsan, her zaman bir tam sayı dönmek zorundadır. Ancak ya Stack boşsa? Python'da böyle bir durumda kolayca return None diyebilirsin. C++'ta bir tam sayı tipinden None dönemezsin.
//İşte std::optional<int> burada devreye girer. Bu, "İçinde ya bir int var ya da hiçbir şey yok (nullopt - Python'daki None gibi)" anlamına gelen güvenli bir kutudur. Kutu doluysa .value() ile içindeki sayıyı alırsın, boşsa içinden hiçbir şey çıkmaz.
//
//const int& (Sadece Okunabilir Bellek Adresi):

//int: Dönecek verinin bir tam sayı olduğunu söyler.

//& (Reference - Referans): C++'ta değişkenin sonuna eklenen & işareti, "Bu değerin kopyasını oluşturma, bana direkt bellekteki (RAM) orijinal adresini ver" demektir.

//const: "Bu adrese gidip bakabilirsin ama içindeki değeri değiştiremezsin" garantisidir.

//xÖzetle fark: std::optional<int> verinin kendisini (kopyasını) barındıran güvenli bir kutuyken, const int& sadece başka bir yerde duran verinin adresini gösteren bir tabeladır.


//return top.value(); KISMININ ÖZETİ
//Yani return top.value(); ifadesi, fonksiyonu çağıran yere silinmek üzere olan bir verinin harita konumunu döndürüyordu. 
//Biz de çözümü, adresi değil değerin direkt kopyasını (by value) döndürmekte bulduk.