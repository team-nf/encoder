/*
This file is part of ncoder.
Copyright (C) 2022 Tuna Gül, Yüşa Furkan Erenci

SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef _CALIB_H_INCLUDED
#define _CALIB_H_INCLUDED

#include "header.h"


#define _eeprom_address_g 0
#define _calibration_start_g "calibeg"
#define _version_skip_key_g "9999999"


typedef struct calibration_meta_t {
	char calibration_start[8];
	char version[8];
	int sensor_num;
} calibration_meta_t;

typedef struct calibration_data_t {
	calibration_meta_t header;
	circle_t magnet_projection;
	sensor_data_t sensor_datas[_sensor_num_g];
} calibration_data_t;


const calibration_meta_t _example_meta_g = { 
	_calibration_start_g, _version_g, _sensor_num_g };


#ifndef _ARDUINO
#warning CALIBRATION NOT IMPLEMENTED FOR PLATFORM

#else
#include <EEPROM.h>
	

#define calibration_read(start_address) \
	(calibration_data_t *)eeprom_read(start_address, sizeof(calibration_data_t))
byte* eeprom_read(int start_address, int size);


#define calibration_write(start_address, buffer) \
	eeprom_write(start_address, (byte *)buffer, sizeof(calibration_data_t))
void eeprom_write(int start_address, void* buffer, int size);



#define _calibration_check_eeprom() _calibration_check_eeprom(_eeprom_address_g, &_example_meta_g)
bool calibration_check_eeprom(int eeprom_address, const calibration_meta_t* example_meta);
bool calibration_check(const calibration_data_t* data, const calibration_meta_t* example_meta);

bool calibration_check_exists(const calibration_meta_t* data_header, const calibration_meta_t* example_meta);
bool calibration_check_sensor_data(const sensor_data_t* sensor_data, int sensor_num);
bool calibration_check_equals(const calibration_data_t* data1, const calibration_data_t* data2);

dbg(void calibration_print(const calibration_data_t* data));


#define _calibrate_sensors(_data) calibrate_sensors(_data, _sensor_pins_g)
bool calibrate_sensors(calibration_data_t* _data, const int* sensor_pins);



#ifdef CALIB_IMPL

byte* eeprom_read(int start_address, int size) {
	byte *buffer = (byte *)malloc(size);
	for (size_t i=0; i < size; i++) {
		buffer[i] = EEPROM.read(start_address + i);
	} return buffer;
}

void eeprom_write(int start_address, byte* buffer, int size) {
	for (size_t i=0; i < size; i++) {
		EEPROM.write(start_address + i, buffer[i]);
	}
}

bool calibration_check_exists(const calibration_meta_t* data_header, const calibration_meta_t* example_meta) {
	if (strcmp(example_meta->calibration_start,	data_header->calibration_start) != 0) { return false; }
	if (strcmp(example_meta->version,			data_header->version		  ) != 0 && \
	    strcmp(example_meta->version,			_version_skip_key_g			  ) != 0) { return false; }
	if (	   example_meta->sensor_num !=		data_header->sensor_num				 && \
			   example_meta->sensor_num !=		0									) { return false; }
	return true;
}

bool calibration_check_sensor_data(const sensor_data_t* sensor_datas, int sensor_num) {
	for (int i=0; i < sensor_num; i++) {
		if (0 > sensor_datas[i]._min || sensor_datas[i]._min > 1023) { return false; } 
		if (0 > sensor_datas[i]._max || sensor_datas[i]._max > 1023) { return false; } 
		if (0 >= sensor_datas[i]._normal || sensor_datas[i]._normal >= 1023) { return false; } 
		if (sensor_datas[i]._min >= sensor_datas[i]._normal || sensor_datas[i]._normal >= sensor_datas[i]._max) { return false; } 
	} return true;
}

bool calibration_check_equals(const calibration_data_t* data1, const calibration_data_t* data2) {
	if (calibration_check_exists(&data1->header, &data2->header) == false)		{ return false; }
	if (data1->magnet_projection.center.x != data2->magnet_projection.center.x) { return false; }
	if (data1->magnet_projection.center.y != data2->magnet_projection.center.y) { return false; }
	if (data1->magnet_projection.radius != data2->magnet_projection.radius)		{ return false; }
	for (int i=0; i < data1->header.sensor_num; i++) {
		if (data1->sensor_datas[i]._min != data2->sensor_datas[i]._min)			{ return false; }
		if (data1->sensor_datas[i]._max != data2->sensor_datas[i]._max)			{ return false; }
		if (data1->sensor_datas[i]._normal != data2->sensor_datas[i]._normal)	{ return false; }
	} return true;
}

bool calibration_check_eeprom(int eeprom_address, const calibration_meta_t* example_meta) {
	calibration_data_t *read_data = calibration_read(eeprom_address);
	bool rv = calibration_check(read_data, example_meta);
	free(read_data); return rv;
}

bool calibration_check(const calibration_data_t* data, const calibration_meta_t* example_meta) {
	if (!calibration_check_exists(&data->header, example_meta)) {
		return false; 
	} if (!calibration_check_sensor_data((sensor_data_t *)&data->sensor_datas, example_meta->sensor_num)) {
		return false;
	} return true;
}

#ifdef _DEBUG
void calibration_print(const calibration_data_t* data) {
	serialf("Start: %s\nVersion: %s\nSensor Count: %d\n", \
		data->header.calibration_start, data->header.version, data->header.sensor_num);
	for (int i=0; i < data->header.sensor_num; i++) {
		serialfn("\tSensor %d: %d %d %d", data->sensor_datas[i]._min, \
			data->sensor_datas[i]._normal, data->sensor_datas[i]._max);
	}
}
#endif

/*bu fonksiyon calibrate_sensor() içerisinde çağrılır.*/
/*DÜZELT, sensor_data_t olmaz farklı bir şey lazım.*/
bool calc_magnet_projection(calibration_data_t* _data, sensor_data_t* _sensor_data, int sensor_num) {
/*burda yapmam gereken _data.magnet_projection'ı sensörlerden aldığım veriye göre hesaplamak*/
	serialdn("Calculating magnet projection...\n");
	for (i = 0; i < sensor_num; i++) {
		for (j = 1; j < sensor_num; j++) {
			for (k = 2; j < sensor_num; k++) {
				/*max - min*/
				serialdn("in loop");
		}
	}
}	
#endif
}

bool calibrate_sensors(calibration_data_t* _data, const int* sensor_pins) {
	/* Eğer debug modundaysak gönderilen datayı kontrol et */
	/* dbg(if (!calibration_check_exists(&_data->header, &_example_meta_g)) { return false; }) */

	/* Eğer verilen datada hata varsa düzelt */
	if (!calibration_check_exists(&_data->header, &_example_meta_g)) {
		serialdn("Calibration header not valid\nRewriting...");
		_data->header = _example_meta_g;
	}


	/* Biraz da olsun hız kazanmak için */
	int sensor_num = _data->header.sensor_num;
	/* Okunan değerleri kaydetmek için kullanılacak array */
	sensor_data_t *read_values = (sensor_data_t *)malloc(sensor_num * sizeof(sensor_data_t));

	/* ilk okuma (normal değerleri anlamak için) */
	for (int i=0; i < sensor_num; i++) {
		int _pin_val = analogRead(sensor_pins[i]);
		read_values[i] = { _pin_val, _pin_val, _pin_val };
	}

	/* ufak bir işaret */
	blink(100, 30);
	/* counter sensörlerin kaç kere okunduğunu saymak için */
	unsigned long counter = 0, start_time = millis();
	/* toplamda 6 saniye boyunca sensörlerden veri okunacak  */
	/* ve verilerin olası değer aralıkları belirlenecek */
	while (millis() - start_time <= _calibration_length_g * 1000) {
		for (int i=0; i < sensor_num; i++) {
			/* sensörlerin sırayla takılmış olması gerekiyor */
			int read_value = analogRead(sensor_pins[i]);
			/* eğer en küçük değerden daha küçük değer okunursa en küçük değeri değiştir */
			if (read_value < read_values[i]._min)		read_values[i]._min = read_value;
			/* yukardakinin aynısı ama en büyük için */
			else if (read_value > read_values[i]._max)	read_values[i]._max = read_value;
		} counter++;
	/* bittiğine dair sinyal */
	} blink(100, 30);
	

#ifdef _NOSENSOR
	/* eğer sensör takılı değilse normal değeri maxla minin ortalamasına ata */
	for (int i=0; i < sensor_num; i++)
		read_values[i]._normal = (read_values[i]._max - read_values[i]._min) / 2;
#endif

#ifdef _DEBUG
	/* biraz debug info */
	serialfn("Read %u sensors %lu times in %d seconds.", sensor_num, counter, _calibration_length_g);
	for (int i=0; i < sensor_num; i++)
		serialf("Sensor %d:\n\t[min]\t\tread: %d\n\t[normal]\tread: %d\n\t[max]\t\tread: %d\n", i,	\
			read_values[i]._min, read_values[i]._normal, read_values[i]._max);
#endif

	/* BURAYI DÜZELT */
	_data->magnet_projection.center = {0, 0};
	_data->magnet_projection.radius = 0.10;

	/* eğer okunan veri geçerli değilse değişiklikleri verilen dataya yazmadan çık */
	if (calibration_check_sensor_data(read_values, sensor_num) == 0) { 
		serialdn("calibration_check_sensor_data failed");	
		return false;
	}
	/* okunan veri geçerliyse dataya yaz */
	memcpy(_data->sensor_datas, read_values, sensor_num * sizeof(sensor_data_t));

	free(read_values);
	return true;
}

#endif
#endif
#endif
