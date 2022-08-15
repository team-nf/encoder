PROJECT_NAME=encoder
MCU=PICO
# MCU=ARDUINO

PICO_BUILDDIR=build/pico
PICO_CMAKE_GEN_PATH=generate_cmake.py
PICO_MOUNTPATH=/mnt/pico
PICO_MAKEOPTS=-j6
PICO_MAINFILE=src/pico.c



ARD_BOARD=arduino:avr:uno
# ARD_BOARD=arduino:mbed_rp2040:pico
# ARD_BOARD=arduino:mbed:pico

ARD_PORT=/dev/ttyUSB0
ARD_CLI=~/.programs/arduino-cli/arduino-cli
# Makefile ile aynı klasörde olmalı
ARD_INOFOLDER=src
ARD_INOFILE=$(ARD_INOFOLDER)/$(ARD_INOFOLDER).ino
ARD_BUILDDIR=build/arduino

ARD_FLAGS=
ARD_FLAGS+=-b $(ARD_BOARD)
ARD_FLAGS+=--output-dir $(ARD_BUILDDIR)
ARD_FLAGS+=$(ARD_INOFILE)



HEADERS=src/header.h
HEADERS+=src/config.h

HEADERS+=src/analytics.h
HEADERS+=src/calibration.h
HEADERS+=src/calculator.h



ifeq ($(MCU),PICO)
	OUTPUT_FILE=$(PICO_BUILDDIR)/$(PROJECT_NAME).uf2 
	UPLOAD_NEEDED=$(PICO_MOUNTPATH)/INDEX.HTM
	UPLOAD_COMMAND=sudo cp $(PICO_BUILDDIR)/$(PROJECT_NAME).uf2 $(PICO_MOUNTPATH)
endif
ifeq ($(MCU),ARDUINO)
	OUTPUT_FILE=$(ARD_BUILDDIR)/$(ARD_INOFOLDER).ino.hex 
	UPLOAD_NEEDED=$(ARD_PORT)
	UPLOAD_COMMAND=sudo $(ARD_CLI) compile --upload -p $(ARD_PORT) $(ARD_FLAGS)
endif


app: $(OUTPUT_FILE)

upload: $(OUTPUT_FILE) $(UPLOAD_NEEDED)
	$(UPLOAD_COMMAND)

clean:
	rm -rf build/pico/* build/arduino/*

pico: $(PICO_BUILDDIR)/$(PROJECT_NAME).uf2 
arduino: $(ARD_BUILDDIR)/$(ARD_INOFOLDER).ino.hex 

# PICO BUILD COMMANDS
$(PICO_BUILDDIR)/$(PROJECT_NAME).uf2: $(PICO_BUILDDIR)/CMakeCache.txt $(PICO_MAINFILE) $(HEADERS)
	cd $(PICO_BUILDDIR) && make $(PICO_MAKEOPTS) && cd -

$(PICO_BUILDDIR)/CMakeCache.txt: CMakeLists.txt
	rmdir $(PICO_BUILDDIR) ; mkdir $(PICO_BUILDDIR); cd $(PICO_BUILDDIR) && cmake ../.. && cd -

CMakeLists.txt:
	python3 $(PICO_CMAKE_GEN_PATH)


# ARDUINO COMPILE COMMANDS
$(ARD_BUILDDIR)/$(ARD_INOFOLDER).ino.hex: $(ARD_INOFILE) $(HEADERS)
	$(ARD_CLI) compile $(ARD_FLAGS)


