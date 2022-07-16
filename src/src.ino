#include <EEPROM.h>

#include "encoder.h"
#define CALIB_IMPL
#include "calibration.h"

/* 3 pinden 1 saniyede 2976 okuma (arduino uno)*/

calibration_data_t *_calibration_data_g;


bool calib_test(calibration_data_t* _data, int first_sensor) {
#ifdef _DEBUG
	/* Eğer debug modundaysak gönderilen datayı kontrol et */
	if (!calibration_check_exists(&_data->header, &_example_meta_g)) { return false; }
#endif
	/* Biraz da olsun hız kazanmak için */
	int sensor_num = _data->header.sensor_num;
	/* Okunan değerleri kaydetmek için kullanılacak array */
	sensor_data_t *read_values = (sensor_data_t *)malloc(sensor_num * sizeof(sensor_data_t));

	/* ilk okuma (normal değerleri anlamak için) */
	for (int i=0; i < sensor_num; i++) {
		int _pin_val = analogRead(first_sensor+i);
		read_values[i] = { _pin_val, _pin_val, _pin_val };
	}

	/* ufak bir işaret */
	blink(100, 10);
	/* counter sensörlerin kaç kere okunduğunu saymak için */
	unsigned long counter = 0, start_time = millis();
	/* toplamda 6 saniye boyunca sensörlerden veri okunacak  */
	/* ve verilerin olası değer aralıkları belirlenecek */
	while (millis() - start_time <= _calibration_length_g * 1000) {
		for (int i=0; i < sensor_num; i++) {
			/* sensörlerin sırayla takılmış olması gerekiyor */
			int read_value = analogRead(first_sensor+i);
			/* eğer en küçük değerden daha küçük değer okunursa en küçük değeri değiştir */
			if (read_value < read_values[i]._min)		read_values[i]._min = read_value;
			/* yukardakinin aynısı ama en büyük için */
			else if (read_value > read_values[i]._max)	read_values[i]._max = read_value;
		} counter++;
	/* bittiğine dair sinyal */
	} blink(100, 10);
	

#ifdef _NOSENSOR
	/* eğer sensör takılı değilse normal değeri maxla minin ortalamasına ata */
	for (int i=0; i < sensor_num; i++)
		read_values[i]._normal = (read_values[i]._max - read_values[i]._min) / 2;
#endif

#ifdef _DEBUG
	/* biraz debug info */
	serialf("Read %d sensors %lu times in %d seconds.\n", sensor_num, counter, _calibration_length_g);
	for (int i=0; i < sensor_num; i++)
		serialf("Sensor %d:\n\t[min]\t\tread: %d\n\t[normal]\tread: %d\n\t[max]\t\tread: %d\n", i,	\
			read_values[i]._min, read_values[i]._normal, read_values[i]._max);
#endif

	/* eğer okunan veri geçerli değilse değişiklikleri verilen dataya yazmadan çık */
	if (calibration_check_sensor_data(read_values, sensor_num) == 0) { return false; }
	/* okunan veri geçerliyse dataya yaz */
	memcpy(_data->sensor_datas, read_values, sensor_num * sizeof(sensor_data_t));

	free(read_values);
	return true;
}



void setup() {
#ifdef _DEBUG
	Serial.begin(115200);
	Serial.println("\nStarted.");
#endif
	pinMode(LED_BUILTIN, OUTPUT);
}



void loop() {
	Serial.println("Loop begin");
	_calibration_data_g = (calibration_data_t *)malloc(sizeof(calibration_data_t));
	_calibration_data_g->header = _example_meta_g;

	bool rv = calib_test(_calibration_data_g, _first_sensor_pin_g);
	serialf("Return Value: %d\n", rv);

	free(_calibration_data_g);
	Serial.println("End.");
	delay(9000);
}

