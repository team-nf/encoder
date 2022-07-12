app:
	arduino-cli compile src/src.ino -b arduino:avr:uno

upload:
	sudo /home/tunapro/.programs/arduino-cli/arduino-cli compile --upload src/src.ino -b arduino:avr:uno -p /dev/ttyUSB0

monitor:
	sudo /home/tunapro/.programs/arduino-cli/arduino-cli monitor -p /dev/ttyUSB0 -c baudrate=115200
