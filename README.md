# Proje hakkında genel bilgilendirme

Proje şu anlık geliştirme aşamasında olduğundan kullanıma uygun değildir.

Projenin amacı, FIRST Robotics Competition ve Roborio ile uyumlu, fiyatı yüksek olmayan ama kaliteli bir encoder oluşturmak.
Encoder projemiz, açık kaynak kodlu olup herkes tarafından geliştirmeye açıktır. 

Encoderımız step değil analogdur ve bu sebeple diğer encoderlara nazaran motorun kaç derece döndüğünü kesin olarak verebilmektedir.

Bu proje aynı zamanda düzenlenmeye de sonuna kadar açıktır. Eğer encoderınızdan daha kesin sonuçlar almak istiyorsanız ve bunun için 
daha fazla sensör kullanmak isterseniz tek yapmanız gereken şey src klasörü içindeki config.h dosyasındaki `_sensor_num_g` sabitini değiştirmek 
olacakır. `src/config.h` dosyası encoderınızı düzenlemeniz için gereken ayarları içermektedir.

Proje kodları üzerinde yaptığınız değişikliklerin kodlarda hataya sebep olup olmadığını anlamak için kodları pcye uyumlu olacak şekilde derlemek mümkün.
(Detaylar aşağıda)

# Kullanım

Eğer kullandığınız mikrodenetleyici üzerinde eeprom varsa, program öncelikle kalibrasyon yapmaya çalışacaktır. (KALIBRASYON MODU GELİŞTİRMESİ TAMAMLANMADI). 
Eğer eeprom yoksa sensörler hakkındaki bilgilerin elle girilmesi gerekmektedir. (SENSÖRLER HAKKINDA KAYDEDİLEN BİLGİLER DE DEĞİŞEBİLİR).
Kalibrasyon bulunamasına rağmen tekrar kalibre etmek için `src/config.h` dosyasında tanımlanmış olan `calibration_pin_g` sabitinde yazan pin numarasına takılı bir buton (ya da switch) bulunmalıdır.
Switchin takılı olduğu pini değiştirmek için bu değişkeni değiştirebilirsiniz.

Eğer `src/config.h` dosyasında `_DEBUG` sabiti tanımlanmışsa debug çıktıları `screen /dev/tty<PORT>` komutuyla okunabilir.

Kodlar üzerindeki konfigürasyon tamamlandıktan sonra `make` komutu ile kodlar seçtiğiniz mikrodenetleyiciye (ya da pcye) göre derlenir. `make upload` komutu da kodları mikrodenetleyiciye yükler.
Eğer kodları pc için derlediyseniz `make upload` komutu programı çalıştırır.
Arduinoda kodların yüklenmesini sağlamak için `Makefile` dosyası içinden `ARD_PORT` değişkeni değiştirilmelidir.
Aynı şekilde picoya kodların yüklenmesini sağlamak için piconun dosya sisteminin bağlı olduğu path `Makefile` içindeki `PICO_MOUNTPATH` değişkeni içine yazılmalıdır.

## Encoder konfigürasyonu

Encoder üzerinde yapabileceğiniz ayarlamalar şunlardır:
 * Encoder üzerindeki mikrodenetleyiciyi değiştirmek (PC modu da bu kısımda)
 * Encoder üzerindeki sensörlerin takılı olduğu pinleri değiştirmek
 * Encoder üzerindeki sensör sayısını değiştirmek
 * Kodlarda kullanılan ondalıklı sayıların hassasiyetini değiştirmek
 * Eğer mikrodenetleyicinizin üzerinde eeprom varsa sensörler hakkında bilgilerin kaydedildiği konumu değiştirmek (eeprom fazla-okuma yazmadan zarar görmüşse kullanışlı olabilir.)

### Mikrodenetleyiciyi değiştirmek
Ana proje klasöründeki `Makefile` dosyasında 3 tane mikrodenetleyici modu bulunmakta. `MCU=pico`, `MCU=arduino` ve `MCU=pc`.
Mikrodenetleyiciyi değiştirmek için kullandığınız mikrodenetleyicinin olduğu satırı yorum satırından çıkarıp diğerlerini yorum haline getirmelisiniz.

Arduino kullanacaksanız kodlar `arduino-cli` ile derleneceğinden `arduino-cli` kurulu olmalıdır ve `arduino-cli`ın konumu `Makefile` içindeki `ARD_CLI` değişkenine yazılmalıdır.
Kullandığınız arduinonun türü de `Makefile` dosyası içindeki `ARD_BOARD` değişkeni ile seçilir. Aynı zamanda `Makefile` dosyası içineki `ARD_PORT` değişkeninin, arduinonun bağlandığı porta göre ayarlanmış olması gerekmektedir.

Pico kullancaksanız `Makefile` dosyası içindeki `PICO_MOUNTPATH` değişkeni piconuzu bağladığınız pathe göre değiştirilmelidir. Eğer kodları manuel yüklemek isterseniz `build/pico/encoder.uf2` dosyasını piconun içine kopyalayabilirsiniz.

### Sensörlerin takılı olduğu pinleri değiştirmek
DÜZENLEME GEREKİYOR

### Sensör sayısını değiştirmek
`src/config.h` dosyası içindeki `_sensor_num_g` sabiti değiştirilmelidir. En az 3 sensör önerilir.

### Kodlarda kullanılan ondalıklı sayıların hassasiyetini değiştirmek
Eğer kodlarda kullanılacak ondalıklı sayı hassasiyetini arttırırsanız kodların mikrodenetleyicinizin raminde kaplayacağı yer daha fazla olacaktır, ve hassasiyetin artmasına rağmen hızın azalmasına sebep olabilir.
Kodlarda kullanılcak ondalıklı sayı tipini değiştirmek için `src/config.h` dosyası içinden istediğiniz tipi yorum satırından çıkarın ve diğerlerini yorum satırına alın.

### Sensörler hakkında bilgilerin eeprom üzerinde kaydedildiği konumu değiştirmek
`src/calibration.h` içindeki `_eeprom_address_g` sabiti değiştirilmelidir.




