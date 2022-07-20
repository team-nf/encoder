#define _DEBUG

#ifdef _DEBUG
#define dbg(a) a
#endif

#ifndef _DEBUG
#define dbg(a)
#endif

void setup() {
	Serial.begin(115200);
	Serial.println("Started.");
}

void loop() {
	Serial.println("loop");
	dbg(if (1) { Serial.println("debug"); });
	delay(6000);
}
