#include <EEPROM.h>

#define _calibration_pin_g 2
#define _eeprom_address_g 0
#define _sensor_num_g 3
#define _DEBUG

#define _calibration_start_g "calibrationstart"
#define _calibration_end_g "calibration_end_"
#define _version_g "00000000"



enum encoder_mode_g {
	em_analog,
	em_digital,
	em_analog_digital,
	em_count
};


typedef struct sensor_data_t {
	int _min, _normal, _max;
} sensor_data_t;

/* basit bir fonksyion sürekli kullanım için geliştirilmedi */
#define init_sensor_datas(_min, _normal, _max) \
	_init_sensor_datas({_min, _normal, _max}) 

sensor_data_t* _init_sensor_datas(sensor_data_t limits) {
	sensor_data_t new_data[_sensor_num_g];
	for (int i=0; i < _sensor_num_g; i++)
		new_data[i] = limits;
	return new_data;
}


typedef struct calibration_data_t {
	char calibration_start[16];
	char version[8];
	int sensor_num;
	sensor_data_t sensor_datas[_sensor_num_g];
	char calibration_end[16];
} calibration_data_t;


/* datanın tutulacağı yer */
const sensor_data_t *_calibration_data_g;

const int _first_sensor_pin_g						= A0;
enum encoder_mode_g _mode_g							= em_analog_digital;

/* _check
 * kalibrasyon okunmadan ya da yazılmadan önce calibration_check fonksiyonu ile data kontrol edilmeli
 *
 * TODO:
 *
 * eeprom read
 * kalibrasyon modu
	 * calibration read and write check for fail
	 *
 * _init_calibration_data struct init gnu-stylemış onu değiştir 
 */


#define eeprom_read(start_address) \
	(calibration_data_t *)_eeprom_read(start_address, sizeof(calibration_data_t))

void* _eeprom_read(int start_address, int size) {
	byte *buffer = (byte *)malloc(size);
	for (size_t i=0; i < size; i++) {
		buffer[i] = EEPROM.read(start_address + i);
	} return (void *)buffer;
}


#define eeprom_write(start_address, buffer) \
	_eeprom_write(start_address, (void *)buffer, sizeof(calibration_data_t))

void _eeprom_write(int start_address, void* buffer, int size) {
	for (size_t i=0; i < size; i++) {
		EEPROM.write(start_address + i, *((byte *)buffer + i));
	}
}


#define calibration_check() \
	_calibration_check(_eeprom_address_g, _calibration_start_g, _version_g, _sensor_num_g, _calibration_end_g)

bool _calibration_check(int eeprom_address, char calibration_start[16], char version[8], int sensor_num, char calibration_end[16]) {
	/* BURAYA BAK eeprom read ne döndürüyor bilmiyorum */
	calibration_data_t *read_data = eeprom_read(eeprom_address);
	if (strncmp(calibration_start,	read_data->calibration_start, 16)) { return false; }
	if (strncmp(version,			read_data->version		    , 8)) { return false; }
	if ( 	    sensor_num !=		read_data->sensor_num	        ) { return false; }
	if (strncmp(calibration_end,	read_data->calibration_end  , 16)) { return false; }


	sensor_data_t sensor_data;
	for (int i=0; i < sensor_num; i++) {
		sensor_data = read_data->sensor_datas[i]; 
		if (0 >= sensor_data._min || sensor_data._min >= 1023) { return false; } 
		if (0 >= sensor_data._max || sensor_data._max >= 1023) { return false; } 
		if (0 >= sensor_data._normal || sensor_data._normal >= 1023) { return false; } 
		if (sensor_data._min >= sensor_data._normal || sensor_data._normal >= sensor_data._max) { return false; } 
	} return true;
}


#define calibration_read(buffer) \
	_calibration_read(buffer, _eeprom_address_g, _sensor_num_g)

bool _calibration_read(sensor_data_t* buffer, int eeprom_address, int sensor_num) {
#ifdef _DEBUG
	if(!calibration_check()) { 
		Serial.println("Check before you read!"); 
		return false;
	}
#endif
	calibration_data_t *read_data = eeprom_read(eeprom_address);
	memcpy(buffer, read_data->sensor_datas, sensor_num * sizeof(sensor_data_t));
	return true;
}


#define calibration_write(sensor_datas) \
	_calibration_write(sensor_datas, _eeprom_address_g, _calibration_start_g, _version_g, _sensor_num_g, _calibration_end_g)

bool _calibration_write(sensor_data_t sensor_datas[_sensor_num_g], int eeprom_address, \
	char calibration_start[16], char version[8], int sensor_num, char calibration_end[16]) 
{
	calibration_data_t data = {calibration_start, version, sensor_num, sensor_datas, calibration_end};
	eeprom_write(eeprom_address, &data);
	return true;
}

bool calibrate_sensors(sensor_data_t* buffer, int first_sensor, int sensor_num, enum encoder_mode_g mode) {
	return true;
}



void setup() {
#ifdef _DEBUG
	Serial.begin(115200);
	Serial.println("Started.");
#endif

	pinMode(_calibration_pin_g, INPUT);

	_calibration_data_g = (sensor_data_t *)malloc(_sensor_num_g * sizeof(sensor_data_t));

	bool calibration_rv = false;
	// Kalibrasyon istenmiyorsa ve eski kalibrasyonda sıkıntı yoksa, kalibrasyonu atla
	if (digitalRead(_calibration_pin_g) == LOW && calibration_check()) { calibration_rv = true; }
	// Kalibrasyon tamamlanana kadar tekrarla
	while (calibration_rv == false) {
#ifdef _DEBUG
		Serial.println("Starting Calibration...");
#endif
		calibration_rv = calibrate_sensors(_calibration_data_g, _first_sensor_pin_g, _sensor_num_g, _mode_g);
	} 

}

void loop() {
#ifdef _DEBUG
	if (_mode_g == em_analog) {
		Serial.println("analog mode");
	} else if (_mode_g == em_digital) {
		Serial.println("digital mode");
	} else if (_mode_g == em_analog_digital) {
		Serial.println("analog+digital mode");
	} 
#endif
	digitalWrite(LED_BUILTIN, HIGH);
	delay(1000);
	digitalWrite(LED_BUILTIN, LOW);
	delay(1000);
}


