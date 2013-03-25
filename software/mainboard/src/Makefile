# @author haggl
# @version 0.5.3 (10): 2011-11-14

# MCU name
MCU = atmega16

# main oscillator and cpu frequencies
F_OSC = 1000000
F_CPU = 1000000

# optimization level, can be [0, 1, 2, 3, s].
# 0 = turn off optimization. s = optimize for size.
OPT = s

# target file name (without extension).
TARGET = midiSwitcher

# C source files (C dependencies are automatically generated.)
SRC = $(TARGET).c engine.c interface.c com.c

# output directory.
DIR = ../bin/

# fuse bits
HFUSE = 0xd1
LFUSE = 0xe1


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #
# normally you should not need to change the following settings
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #


# compiler flags.
CFLAGS = -mmcu=$(MCU) -I.
CFLAGS += -O$(OPT)
CFLAGS += -Wall -Wstrict-prototypes
CFLAGS += -DF_OSC=$(F_OSC)
CFLAGS += -DF_CPU=$(F_CPU)

# avrdude settings and variables.
AVRDUDE_FLAGS = -B 4
AVRDUDE_FLAGS += -p $(MCU)
AVRDUDE_FLAGS += -c usbtiny

AVRDUDE_WRITE_FLASH = -U flash:w:$(DIR)$(TARGET).hex
AVRDUDE_VERIFY_FLASH = -U flash:v:$(DIR)$(TARGET).hex
AVRDUDE_READ_EEPROM = -U eeprom:r:$(DIR)eeprom.eep:i
AVRDUDE_WRITE_EEPROM = -U eeprom:w:$(DIR)eeprom.eep:i
AVRDUDE_WRITE_FUSES = -U hfuse:w:$(HFUSE):m
AVRDUDE_WRITE_FUSES += -U lfuse:w:$(LFUSE):m


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #
# please do not edit below
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #


# define programs and commands.
SHELL = /bin/sh
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
REMOVE = rm -f
MKDIR = mkdir

# define Messages
MSG_ASSEMBLING = make: *** assembling...
MSG_BYE = make: *** have a nice day!
MSG_CLEANING = make: *** cleaning project...
MSG_COMPILING = make: *** compiling...
MSG_DOWNLOADING = make: *** backing up EEPROM...
MSG_DUMPING = make: *** dumping EEPROM...
MSG_EEPROM = make: *** creating load file for EEPROM...
MSG_FLASH = make: *** creating load file for Flash...
MSG_LINKING = make: *** linking...
MSG_HELLO = make: *** hiho!
MSG_PROGRAMING = make: *** programing MC...
MSG_FUSE = make: *** setting fuse bits...

# default target.
help:
	@echo "#------------------------------------------------------------------------------#"
	@echo "Hello and welcome to haggls awesome universal AVR Makefile! ;-)"
	@echo
	@echo "Here are the available targets:"
	@echo "\tmake all = clean, compile and program"
	@echo "\tmake allinc = backup, clean, compile, program and eepdump"
	@echo "\tmake asm = compile the software and create .asm-file"
	@echo "\tmake backup = read data from eeprom"
	@echo "\tmake build = compile the software and create .hex and .asm-files"
	@echo "\tmake clean = clean up built project files"
	@echo "\tmake dump = write data to eeprom"
	@echo "\tmake fuse = write fuse bits"
	@echo "\tmake help = display this message"
	@echo "\tmake hex = compile the software and create .hex-file"
	@echo "\tmake program = load the hex file to the device, using avrdude"
	@echo
	@echo "Don't forget to adjust the variables in the Makefile header!"
	@echo "AND:"
	@echo "Be careful with fuse bits!"
	@echo "#------------------------------------------------------------------------------#"


# say hello.
hello:
	$(info )
	$(info $(MSG_HELLO))

# say bye.
bye:
	$(info )
	$(info $(MSG_BYE))
	$(info )

# compile: create object files from C source files.
$(DIR)%.o: %.c
	$(info )
	$(info $(MSG_COMPILING) $<)
	$(CC) -c $(CFLAGS) $< -o $@

# link: create ELF output file from object files.
OBJ = $(addprefix $(DIR),$(patsubst %.c,%.o,$(SRC)))
%.elf: $(OBJ)
	$(info )
	$(info $(MSG_LINKING) $@)
	$(CC) $(CFLAGS) $(OBJ) -o $(DIR)$@

# create final output file (.hex) from ELF output file.
hex: $(TARGET).hex
%.hex: %.elf
	$(info )
	$(info $(MSG_FLASH) $@)
	$(OBJCOPY) -O ihex -R .eeprom $(DIR)$< $(DIR)$@

# compile: create assembler files from C source files.
asm: $(TARGET).s
%.s: %.c
	$(info )
	$(info $(MSG_ASSEMBLING) $<)
	$(CC) -S $(CFLAGS) $< -o $(DIR)$@

# target: clean project.
clean: hello makeclean bye
makeclean:
	$(info )
	$(info $(MSG_CLEANING))
	$(REMOVE) $(DIR)$(TARGET).hex
	$(REMOVE) $(DIR)$(TARGET).elf
	$(REMOVE) $(DIR)$(TARGET).s
	$(REMOVE) $(OBJ)

# target: build project
build: hello hex bye

# target: program the device.
program: hello makeprogram bye
makeprogram:
	$(info )
	$(info $(MSG_PROGRAMING))
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH) $(AVRDUDE_VERIFY_FLASH)

# target: set fuse bits.
fuse: hello makefuse bye
makefuse:
	$(info )
	$(info $(MSG_FUSE))
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FUSES)

# target: backup eeprom data
backup: hello makebackup bye
makebackup:
	$(info )
	$(info $(MSG_DOWNLOADING))
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_READ_EEPROM)

# target: eeprom dump
dump: hello makedump bye
makedump:
	$(info )
	$(info $(MSG_DUMPING))
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_EEPROM)

# target: compile and program.
all: hello makeclean hex makeprogram bye

# target: backup, compile, program and dump.
allinc: hello makeclean hex makebackup makeprogram makedump bye

# phony targets.
.PHONY: all allinc asm backup build bye clean dump fuse hello hex makebackup makeclean makedump makefuse makeprogram program


.SECONDARY: $(OBJ)
