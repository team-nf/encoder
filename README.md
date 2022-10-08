# Note

Information on the licensing of ncoder is present at the end of this markdown file in english and turkish.

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

Eğer `src/config.h` dosyasında `_DEBUG` sabiti tanımlanmışsa debug çıktıları `screen /dev/tty<PORT>` komutuyla ya da arduino programının Serial Monitorü yardımıyla okunabilir. (BAUDRATE=115200)

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
DÜZENLEME GEREKİYOR!

### Sensör sayısını değiştirmek
`src/config.h` dosyası içindeki `_sensor_num_g` sabiti değiştirilmelidir. En az 3 sensör önerilir.

### Kodlarda kullanılan ondalıklı sayıların hassasiyetini değiştirmek
Eğer kodlarda kullanılacak ondalıklı sayı hassasiyetini arttırırsanız kodların mikrodenetleyicinizin raminde kaplayacağı yer daha fazla olacaktır, ve hassasiyetin artmasına rağmen hızın azalmasına sebep olabilir.
Kodlarda kullanılcak ondalıklı sayı tipini değiştirmek için `src/config.h` dosyası içinden istediğiniz tipi yorum satırından çıkarın ve diğerlerini yorum satırına alın.

### Sensörler hakkında bilgilerin eeprom üzerinde kaydedildiği konumu değiştirmek
`src/calibration.h` içindeki `_eeprom_address_g` sabiti değiştirilmelidir.


# LICENSING

## Definitions.

"ncoder" refers to this software. ncoder's source code is publicly avaliable on github at <https://www.github.com/NF9029/encoder>

"source code" refers to any file, executable, folder, etc. present at, and/or extracted in any way from, the repository directory at <https://www.github.com/NF9029/encoder>

## License Notice

Copyright (C) 2022 Tuna Gül, Yüşa Furkan Erenci

This file is part of ncoder.
ncoder is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

ncoder is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ncoder (in the file LICENSE present in the root directory of ncoder). If not, see <https://www.gnu.org/licenses/>

## Further Notice

The license notice noted above is in force and covers all elements of the source code in its entirety. Everything that is part of ncoder is licensed under the GNU General Public License, do take knowledge of the terms of the GNU General Public License. The license notice above may or may not be exclipitly included in its entirety in script upon the parts of source code yet everything stated above is still in motion and all parts of ncoder is licensed.

Individual files of ncoder may contain the following tag instead of the full license text.
	"SPDX-License-Identifier:	GPL-3.0-or-later"

This enables machine processing of license information based on the SPDX
License Identifiers that are here available: <http://spdx.org/licenses/>

## Contact Info

The authors are contactable via email at,
Tuna Gül = tunagul54@gmail.com
Yüşa Erenci = iamyusafurkan@gmail.com 


# LISANS BILGILERI

## Tanımlar

Aşağıdakiler yazılar dahilinde,

"ncoder" bu programa itafen kullanılacaktır, ncoder'ın kaynak kodu herkese açık bir şekilde github'da <https://github.com/NF9029/encoder> web adresinde bulunmaktadır;

"kaynak kodu" yukarıda belirtilen <https://github.com/NF9029/encoder> adresinde bulunan ve oradan paylaşılan, dağıtılan veya herhangi bir şekilde elde edilen herhangi dosya, kod, yazı, uygulama dahil içeriğe itafen kullanılacaktır;

"Özgür Yazılım Vakfı" ismi çevirinin okunabilirliğini arttırmak adına, "Free Software Foundation" isimli vakfın ismine itafen kullanılacaktır;

"GNU Genel Kamu Lisansı" ismi çevirinin okunabilirliğini arttırmak adına, GNU General Public License lisansının ismine itafen kullanılacaktır;

Bu tanımlara dikkatiniz arz edilir.

## Lisans Bildirisi

Telif Hakları Saklıdır. 2022 Tuna Gül, Yüşa Furkan Erenci

Bu dosya ncoder'ın bir parçasıdır
ncoder özgür programdır: Özgür Yazılım Vakfı tarafından yayımlanan GNU Genel Kamu Lisansı’nın sürüm 3 ya da (isteğinize bağlı olarak) daha sonraki sürümlerinin hükümleri altında yeniden dağıtabilir ve/veya değiştirebilirsiniz. 

ncoder yararlı olması umudunda dağıtılmış olup, programın HİÇBİR TEMİNATI YOKTUR; TİCARETİNİN YAPILABİLİRLİĞİNE VE ÖZEL BİR AMAÇ İÇİN UYGUNLUĞUNA dair bir teminat da vermez. Ayrıntılar için GNU Genel Kamu Lisansı’na göz atınız.

Bu programla birlikte GNU Genel Kamu Lisansı’nın bir kopyasını elde etmiş olmanız gerekir (LICENSE dosyası). Eğer elinize ulaşmadıysa <http://www.gnu.org/licenses/> adresine bakınız.

## Ek Bildiri

Yukarıda belirtilen lisans bildirisi kaynak kodunu tüm parçaları ve içeriği ile kaplamakta. ncoder programının tümü ve tamamı GNU Genel Kamu Lisansı altında lisanslıdır, haliyle GNU Genel Kamu Lisanları hükümleri hakkında bilinçli olunuz. Yukarıdaki lisans bildirisi yazı olarak tamamı halinde kaynak kodunun parçaları içerisinde açık bir halde bulunmayabilir ancak yukarıda belirtilen her şey geçerlidir ve ncoder'ın tamamı lisanslıdır.

ncoder'ın başlıbaşına dosyaları lisansın tam yazısını içermek yerine şu  etiketi bulundurabilir:
		"SPDX-License-Identifier:GPL-3.0-or-later"

Bu etiket <https://spdx.org/licenses/> adresinde mevcut olam SPDX Lisans Kimliklerine dayanarak lisans bilgisinin makine işlemesini mümkün kılar.

## İletişim Bilgisi

Yazarlara aşağıdaki adresler aracılığında ulaşabilirsiniz:
Tuna Gül = tunagul54@gmail.com
Yüşa Erenci = iamyusafurkan@gmail.com
