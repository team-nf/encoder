/* tunapro1234 */
#define ENCODER_IMPL
#include "encoder.h" 
#define CALIB_IMPL
#include "calibration.h"

/* datanın tutulacağı yer */
const sensor_data_t *_calibration_data_g;


/* _check
 * kalibrasyon okunmadan ya da yazılmadan önce calibration_check fonksiyonu ile data kontrol edilmeli
 *
 * TODO:
 *
 * calibration dump load
 *
 * 
 */


void setup() {
#ifdef _DEBUG
	Serial.begin(115200);
	Serial.println("Started.");
#endif

	pinMode(_calibration_pin_g, INPUT);

	_calibration_data_g = (sensor_data_t *)malloc(_sensor_num_g * sizeof(sensor_data_t));

	bool calibration_rv = false;
	// Kalibrasyon istenmiyorsa ve eski kalibrasyonda sıkıntı yoksa, kalibrasyonu atla
	if (digitalRead(_calibration_pin_g) == LOW && calibration_check_eeprom()) { calibration_rv = true; }
	// Kalibrasyon tamamlanana kadar tekrarla
	while (calibration_rv == false) {
#ifdef _DEBUG
		Serial.println("Starting Calibration...");
#endif
		calibration_rv = calibrate_sensors(_calibration_data_g);
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


