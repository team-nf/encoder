app:
	arduino-cli compile src/src.ino -b arduino:avr:uno
# --build-property "build.extra_flags=\"-fpermissive\""

upload:
	sudo /home/tunapro/.programs/arduino-cli/arduino-cli compile --upload src/src.ino -b arduino:avr:uno -p /dev/ttyUSB0
# 	sudo /home/tunapro/.programs/arduino-cli/arduino-cli upload src/src.ino -b arduino:avr:uno -p /dev/ttyUSB0 && echo "done."

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
		src/src.ino

# -tools /usr/share/arduino/hardware/tools-builder \
