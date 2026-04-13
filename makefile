NAME=main
CC=avr-gcc
FLAGS=-nostdlib -mmcu=atmega328p
F_CPU=16000000UL
HEX=avr-objcopy
HEX_FLAGS= -I binary
HEX_OUT= ihex
FLASH=avrdude
PROGRAMMER= -c arduino
CARD= -p atmega328p
PORT= -P /dev/ttyUSB0
BAUD= -b 115200
FLASH_IN_OUT= -U flash:w:$(NAME).hex:i


all: hex

hex:
	$(CC) $(FLAGS) -DF_CPU=$(F_CPU) -o $(NAME).bin $(NAME).c
	$(HEX) $(HEX_FLAGS) -O $(HEX_OUT) $(NAME).bin $(NAME).hex
 
flash:
	$(FLASH) $(PROGRAMMER) $(CARD) $(PORT) $(BAUD) $(FLASH_IN_OUT)

clean:
	rm -rf $(NAME).bin
	rm -rf $(NAME).hex


.phony: all hex flash clean


