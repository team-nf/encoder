ino_file = src/main.ino

app:
	arduino-cli compile $(ino_file) -b arduino:avr:uno
# --build-property "build.extra_flags=\"-fpermissive\""

upload:
	sudo /home/tunapro/.programs/arduino-cli/arduino-cli compile --upload $(ino_file) -b arduino:avr:uno -p /dev/ttyUSB0
# 	sudo /home/tunapro/.programs/arduino-cli/arduino-cli upload $(ino_file) -b arduino:avr:uno -p /dev/ttyUSB0 && echo "done."

monitor:
	sudo /home/tunapro/.programs/arduino-cli/arduino-cli monitor -p /dev/ttyUSB0 -c baudrate=115200

builder: 
	arduino-builder \
		-compile \
		-hardware /usr/share/arduino/hardware \
		-tools /usr/share/arduino/hardware/tools \
		-libraries lib \
		-fqbn arduino:avr:uno \
		-build-path build \
		$(ino_file)

# -tools /usr/share/arduino/hardware/tools-builder \
