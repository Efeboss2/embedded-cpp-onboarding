#include <gtest/gtest.h>
#include "static_stack.hpp"
#include <iostream>
#include "raw_buffer.hpp"
#include "static_ring_buffer.hpp"


/*

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
std::optional<Sensor> get_top_value(const StaticStack& stack) { //int yerine Sensor döndürüyoruz exapmle 3.1 için
    return stack.peek(); // Direkt güvenli kopyasını döndürüyoruz 
}

//TEST(Part2UBTest, SafeValueReturnExample) {
//    StaticStack stack;
//    stack.push(42);

    // Artık silinmiş bir adres değil, gerçek bir kopyaya (value) sahibiz
//    std::optional<int> safe_val = get_top_value(stack);
    
//    // Değerin başarıyla geldiğini ve tam olarak 42 olduğunu doğruluyoruz
//    EXPECT_TRUE(safe_val.has_value());
//    EXPECT_EQ(safe_val.value(), 42); 
//}// Bu test, artık Undefined Behavior içermiyor ve her zaman geçecektir.


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


//**The rule:** if a method does not modify the object's state, mark it `const`. Always. This is not optional hygiene — in template code,
// `const` incorrectness causes cascading errors that are genuinely difficult to trace.

//


TEST(StaticStackTest, PushOneElementChangesSize) {
    StaticStack stack;
    
    // 1. Önce bir Sensor objesi oluştur
    Sensor s1{1, 25.5f, "TempSensor"}; 
    
    // 2. Sonra bu objeyi push et
    stack.push(s1); 
    
    EXPECT_EQ(stack.size(), 1u);
    
    // İstersen pop yapıp değerleri de doğrulayabilirsin
    auto popped = stack.pop();
    ASSERT_TRUE(popped.has_value());
    EXPECT_EQ(popped.value().id, 1);
}
/* 
// Bu fonksiyon derleme (compile-time) testi içindir.
// Eğer read-only metotlar 'const' olarak işaretlenmediyse, derleyici hata fırlatır.
void verify_const_correctness(const StaticStack <Sensor, 16> template kullanıyorsan ekle & const_stack) {
    auto top = const_stack.peek();
    bool empty = const_stack.is_empty();
    bool full = const_stack.is_full();
    std::size_t count = const_stack.size();
    
    // (Opsiyonel) Değişkenleri kullanmadığımız için derleyicinin "unused variable" 
    // uyarısı (warning) vermesini engellemek adına:
    (void)top; (void)empty; (void)full; (void)count;
}

TEST(StaticStackTest, ConstCorrectnessIsSound) {
    StaticStack stack;
    verify_const_correctness(stack);
    SUCCEED(); // Buraya kadar derlenip geldiyse test başarılıdır.
}
// Lokal bir değişken oluşturup pointer'ını stack'e kopyalayarak UB yaratan fonksiyon
StaticStack create_stack_with_dangling_pointer() {
    StaticStack stack;
    
    // local_name sadece bu fonksiyonun scope'unda yaşar
    char local_name[] = "TempSensor"; 
    
    Sensor s{1, 25.5f, local_name};
    stack.push(s);
    
    // stack by value olarak döner (kopyalanır). 
    // s kopyalanır ama 'name' pointer'ı sığ kopyalama (shallow copy) olduğu için
    // hala artık silinecek olan local_name'in bellek adresini gösterir.
    return stack; 
}

TEST(StaticStackTest, SensorNameLifetimeBug) {
    auto buggy_stack = create_stack_with_dangling_pointer();
    
    auto top_sensor = buggy_stack.peek();
    ASSERT_TRUE(top_sensor.has_value());
    
    // Aşağıdaki satır bir dangling pointer'ı okumaya çalışır (Undefined Behavior).
    // Ekranda "TempSensor" yerine garbage (çöp) değerler görebilirsin.
    std::cout << "\n--- UB GÖZLEMİ ---" << std::endl;
    std::cout << "Sensor name: " << top_sensor.value().name << std::endl;
    std::cout << "------------------\n" << std::endl;
}
*/
/*
 * Exercise 3.2 Observation:
 * When a Sensor is created with a `name` pointing to a local array on the stack, 
 * returning the StaticStack destroys the local array. The `name` member inside the 
 * copied Sensor becomes a dangling pointer. It is NOT safe to use after the 
 * function returns. Accessing it results in Undefined Behavior (UB).
 */
TEST(StaticStackTest, SensorReadingMoveTest) {
    
    // Explicitly constructible objemizi oluşturuyoruz
    // SensorReading sr{1, 25.5f, 123456};
    StaticStack<SensorReading, 16> stack;
    // sr explicitly non-copyable olduğu için std::move ile (rvalue olarak) 
    // stack'in içine taşıyoruz (stealing the resources).
    // NOT: sr objesi bu satırdan sonra moved-from state (içi boşaltılmış) durumdadır.
    SensorReading sr{1, 25.5f, 123456};
    bool push_success = stack.push(std::move(sr)); 
    EXPECT_TRUE(push_success);
    
    // pop() metodu da objeyi kopyalamaz, std::move ile dışarı taşır.
    auto popped = stack.pop();
    ASSERT_TRUE(popped.has_value());
    EXPECT_EQ(popped.value().sensor_id, 1);

    
    
    // Değerlerin doğru taşındığını doğruluyoruz (validation)
    EXPECT_EQ(popped.value().sensor_id, 1);
    EXPECT_FLOAT_EQ(popped.value().value, 25.5f);
    EXPECT_EQ(popped.value().timestamp_ms, 123456u);
    
    // Eleman pop edildiği için stack tekrar boş olmalı
    EXPECT_TRUE(stack.is_empty());
}
// EXERCISE 5.2: Copy Testi
TEST(RawBufferTest, DeepCopyLeavesOriginalUnchanged) {
    RawBuffer b1(10); // 10 byte'lık yer ayır
    b1.data[0] = 42;  // İlk byte'a 42 yaz

    RawBuffer b2 = b1; // Copy constructor çalışır
    b2.data[0] = 99;   // Kopyanın ilk byte'ını değiştir

    // Orijinalin değişmediğini (Deep Copy yapıldığını) doğrula
    EXPECT_EQ(b1.data[0], 42);
    EXPECT_EQ(b1.size, 10u);
}
// 2. EXERCISE 6.1: Konsepti (Concept) İhlal Eden Test
// Bu struct default_initializable değil (boş yaratılamaz)!
struct UnstackableType {
    int data;
    UnstackableType() = delete; // Default constructor YASAKLANDI 
};
// EXERCISE 5.2: Move Testi
TEST(RawBufferTest, MoveLeavesSourceEmpty) {
    RawBuffer b1(10);
    b1.data[0] = 42;

    // std::move ile b1'in içindeki pointer'ı b2'ye çalıyoruz
    RawBuffer b2 = std::move(b1); // Move constructor çalışır

    // Hedef obje veriyi başarıyla aldı mı?
    EXPECT_EQ(b2.data[0], 42);
    EXPECT_EQ(b2.size, 10u);

    // Kaynak obje (b1) etkisiz hale getirildi mi? (Dangling pointer engellendi mi?)
    EXPECT_EQ(b1.data, nullptr);
    EXPECT_EQ(b1.size, 0u);
}
TEST(StaticStackTest, ConceptViolationTest) {
    // BURASI PATLAYACAK: UnstackableType, Stackable konseptine uymuyor!
    // Gözlemlemek için alttaki satırın başındaki yorum işaretlerini (//) kaldır:
    
   // StaticStack<UnstackableType, 16> bad_stack; 
}
TEST(StaticRingBufferTest, OverwritesOldestWhenFull) {
    // Sadece 2 eleman alabilen bir Ring Buffer oluşturuyoruz
    StaticRingBuffer<SensorReading, 2> ring;
    
    // Geçici objeleri (rvalue) doğrudan std::move'a gerek kalmadan gönderebiliriz.
    // 3 adet sensör verisi yolluyoruz. Kapasite 2 olduğu için 1 numaralı veri ezilmelidir.
    ring.push(SensorReading{1, 10.0f, 100});
    ring.push(SensorReading{2, 20.0f, 200});
    ring.push(SensorReading{3, 30.0f, 300}); 
    
    // Kuyruk dolu mu? (Kapasite 2 olduğu için boyutu 2'de kalmalı)
    EXPECT_TRUE(ring.is_full());
    EXPECT_EQ(ring.size(), 2u);
    
    // Kuyruktan çıkan ilk eleman 2 numaralı sensör olmalıdır (Çünkü 1 numara ezildi)
    auto first_out = ring.pop();
    ASSERT_TRUE(first_out.has_value());
    EXPECT_EQ(first_out.value().sensor_id, 2);
    
    // Çıkan ikinci eleman 3 numaralı sensör olmalıdır
    auto second_out = ring.pop();
    ASSERT_TRUE(second_out.has_value());
    EXPECT_EQ(second_out.value().sensor_id, 3);
}