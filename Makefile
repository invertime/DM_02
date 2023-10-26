$(DEBUG).SILENT: ;

BUILD=build
SRC=src
C_COMPILER=gcc
C_OPTIONS=-Wall -g
C_LIBS_FLAGS=

$(BUILD)/main.o: $(SRC)/main.c
	if [ ! -d "build" ]; then mkdir build; fi
	echo "Compiling main file..."
	d=$$(date +%s) \
	; $(C_COMPILER) -o $(BUILD)/main.o $(SRC)/main.c $(C_OPTIONS) $(C_LIBS_FLAGS)\
	&& echo "Build took $$(($$(date +%s)-d)) seconds"
