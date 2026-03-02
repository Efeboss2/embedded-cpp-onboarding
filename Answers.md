# Part 1 - Written Questions
Yapay zeka ile metini oluşturdum ancak aralardaki hataları vs kendi cümlelerimi kullanarak değiştirdim ve anladığım şeyleri ekledim. 
**Toolchain**

**1. What does `meson setup build` actually do? What lives in the `build` directory afterward, and why is it separate from the source tree?**
Araştırdığım kadarıyla meson ve ninja farklı şeyler ikisinin de görevi farklı. Meson projeyi compile etmiyor compilera bir plan oluşturuyor. Bu planda test vs gibi bağımlılıkları clang ve linkerların nerede olduğunu vs detaylı bir compile planı çıkarıtyor bu dosyada build.ninja dosyası oluyor. `meson setup build` komutu, part1 klasöründeki `meson.build` dosyasını okuyarak sistemde kullanılacak olan compilera hangi cpp modeli kullaılacağını dependice leri falan analiz ediyo ve ninja için bir derleme planı oluşturur ve bu dosyayı build klasörüne koyuyormuş. İşlem sonrasında `build` klasörünün içinde Ninja'nın kullanacağı `build.ninja` dosyası, derlenmiş obje dosyaları (`.o`) ve `test_static_stack` gibi executable'lar yer alır. Bunun source tree  klasöründen ayrı tutulması, kaynak kodlarımızın derleme çıktılarıyla kirlenmesini önler ve build klasörünü sildiğimizde projeyi anında tertemiz bir duruma getirmemizi sağlar. Kısaca daha temiz derli toplu bir sistem oluşturmuş oluyor. Ayrıca ninja direkt .hpp üzerinden compile edemiyor bunun için .cpp dosyasına çağırıyoruz ve oradaki test durumlarında .hpp dosyasındaki kodu çalıştırıp testi değerlendiriyoruz. .hpp yi çağırmasınıda test_static_stack.cpp içinde çağırdım. 

**2. In the build files, the stack is compiled as a `static_library` rather than being compiled directly into the test executable. What is the practical difference, and why might this separation matter as a project grows?**
`static_library` olarak derlemek, yazdığımız stack veri yapısını bağımsız, paketlenmiş bir modül haline getirir. Doğrudan executable içine compile etseydik, kod sadece o teste özel olurdu. Proje büyüdüğünde ve bu stack yapısını başka executable'larda (örneğin asıl embedded yazılımın içinde) kullanmak istediğimizde, kodu tekrar tekrar derlemek yerine bu statik kütüphaneyi (modülü) direkt olarak link'leyebiliriz (bağlayabiliriz). Bu da hem modülerliği artırır hem de build sürelerini kısaltır.

**C++ Fundamentals**

**3. What is `constexpr` and why is `CAPACITY` declared as `static constexpr` rather than just `const`? Could it be a regular constructor argument instead — and if not, why not?**
`constexpr`, değerin compile-time sırasında kesin olarak bilinmesini ve hesaplanmasını zorunlu kılar. `CAPACITY` değişkenini `static constexpr` yapmak, bu değerin sınıfın her objesi için hafızada ayrı ayrı yer kaplamamasını (benim alanım 16 bit diye her constexpr de hafızaya yazmasını düzletiyor) ve derleme aşamasında `std::array`'in boyutunu belirleyebilmesini sağlar. Normal bir constructor argument yapamayız çünkü `std::array`'in boyutu runtime değişkenleriyle değil, compile-time sabitleriyle belirlenmek zorundadır. Aksi takdirde dinamik bellek kullanmamız gerekirdi.

**4. What is `std::optional` and what problem does it solve here compared to returning a sentinel value like `-1` or `0`?**
`std::optional`, bir değerin var olup olmadığını güvenli bir şekilde ifade eden modern bir C++ wrapper'ıdır. Eğer stack boşken `pop()` fonksiyonu `-1` gibi bir sentinel value döndürseydi, kullanıcı stack'e gerçekten `-1` eklediğinde, dönen değerin bir hata mı yoksa geçerli bir veri mi olduğunu ayırt edemezdik. `std::optional` kullanarak değerin olmama durumunu (`std::nullopt`) geçerli veri kümesinden tamamen izole etmiş oluyoruz. Bu sayede stack de değer var olduğunu bilerek içindeki value nun -1 olduğu durumda da sağlıklı bir şekilde valueyı pop layabilir.  

**5. What does `{}` do in `std::array<int, CAPACITY> _data{}`? What would happen without it?**
`{}` (value initialization / zero initialization), array içindeki tüm elemanların başlangıçta güvenli bir şekilde `0` (sıfır) değeriyle başlatılmasını sağlar. Eğer bunu kullanmasaydık, array'in içindeki kutular hafızada o an rastgele bulunan anlamsız çöpverilerle dolu olurdu. Bu da potansiyel güvenlik ve logic hatalarına yol açabilirdi.

**Looking Forward**

**6. Right now the stack only works for `int` and has a fixed capacity of 16. If you needed two stacks — one for `float` with capacity 8 and one for `int` with capacity 32 — how would you approach that without duplicating the class? You don't need to write code, just describe your thinking.**
Sınıfı kopyalayıp yapıştırmak yerine C++ templates mimarisini kullanırım. Sınıfı tanımlarken hem veri tipini (`typename T`) hem de kapasiteyi (`std::size_t Capacity`) template parametreleri olarak dışarıdan alacak şekilde genel bir `template <typename T, std::size_t Capacity>` iskeleti kurarım. Böylece objeyi oluştururken `StaticStack<float, 8>` veya `StaticStack<int, 32>` yazarak tek bir kod tabanından farklı özelliklerde statik yığınlar üretebilirim.

template <typename T, std::size_t Capacity> isim; bu bizim templateimiz

kodun çinde biryerde 8 bit lik float stacki oluşturmak istediğimde 
StaticStack<float, 8> sensor_verileri;

buda  32bitlik int stacki kurmak istediğimizde
StaticStack<int, 32> islem_kuyrugu;