
#define serialf(...) ({ \
	int _size = snprintf(NULL, 0, __VA_ARGS__) + 1; \
	char* _str = (char *)malloc(_size * sizeof(char)); \
	snprintf(_str, _size, __VA_ARGS__); \
	Serial.print(_str); \
	free(_str); \
})

void setup() {
	Serial.begin(115200);
	Serial.println("Started.");
}

void loop() {
	int first_pin = A0;
	int sensor_num = 3;
	for (int i=0; i < sensor_num; i++)
		serialf("AnalogRead %d: %d\t", i, analogRead(first_pin+i));
	serialf("\n");
	delay(500);
}
