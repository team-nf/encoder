/*
This file is part of ncoder.
Copyright (C) 2022 Tuna Gül, Yüşa Furkan Erenci

SPDX-License-Identifier: GPL-3.0-or-later
*/

#define _sensor_num_g 1
const int _sensor_pins_g[] = {A0};

void setup() {
	Serial.begin(115200);
	Serial.println("Started");
	for (int i = 0; i < _sensor_num_g; i++) {
		pinMode(_sensor_pins_g[i], INPUT);
	}
}

void loop() {
	for (int i = 0; i < _sensor_num_g; i++) {
		Serial.print(analogRead(_sensor_pins_g[i]));
		Serial.print(" ");
	} 
	Serial.println("");
	delay(20);
}
