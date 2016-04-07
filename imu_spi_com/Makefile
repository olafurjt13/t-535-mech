#
# Arduino code makefile. Has to be run as sudo
#

# Object file output name
NAME = imu_spi_com
OUTN = $(NAME).obj
OUTH = $(NAME).hex

# AVRgcc (compile)
CC = avr-gcc
CFLAGS = -g -Os -Wall -mcall-prologues -fno-exceptions
MOD = -mmcu=atmega328p
OUT = -o $(OUTN)
IN = *.c

# Convert obj to hex
OC = avr-objcopy
HFLAGS = -R .eeprom
OUTP = -O ihex $(OUTN)
INP = $(OUTH)

# AVRdude config (Upload)
UP = avrdude
MODEL = -pm328p 
FLAGS = -carduino
PORT = -P/dev/ttyUSB0
REST = -Uflash:w:$(OUTN):a

# AVR Objectdump
OD = avr-objdump
ODFLAGS = -d $(OUTN)
OUTASM = $(NAME).lss

all:
	$(CC) $(CFLAGS) $(MOD) $(OUT) $(IN)
	$(OC) $(HFLAGS) $(OUTP) $(INP)

ul:
	$(UP) $(MODEL) $(FLAGS) $(PORT) $(REST)

asm:
	$(OD) $(ODFLAGS) > $(OUTASM)

clean:
	rm -f *.o *~ *.obj *.hex *.lss
