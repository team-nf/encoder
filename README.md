# encoder

Makefile içeresinden MCU ayarı yapılabilir. Derlenecek platformu değiştirmek için `MCU= bi şeyler` yazan yerlerle oynayın. 
`MCU= bi şeyler` yazan yerle oynamak istemiyorsanız `make arduino`, `make pico` ya da `make pc` komutlarını da kullanabilirsiniz. 

`make upload` var mcuya yükleme yapıyor pcde kodu çalıştırıyor. 
Picoya yükleme yapmak biraz sıkıntı `make upload` demeden önce piconun /mnt/pico klasörüne mountlı olması gerekiyor.

Çıktıları okumak için `screen /dev/tty<PORT>` komutu kullanılabilir.

Windowsta çalışır mı bilmiyorum denemedim. Make kurmaya üşeniyorum.

Kodu yüklemeden config.h dosyasıyla oynanmalı.

Öyle bi şeyler siz düzenlersiniz buraları ben kod yazmaya gidiyom.


## TODO
 * Memory leak check with valgrind
 * Multiprocessing (pico)
 * Eeprom (pico and pc)
 * Calibration improvement
 * Roborio communication
 * Voltage regulation circuits for pico
