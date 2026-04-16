# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/13 13:45:55 by nrobinso          #+#    #+#              #
#    Updated: 2026/04/14 15:22:26 by nrobinso         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ------------------------- project -------------------------------------------
NAME=main

# ------------------------- compiler ------------------------------------------
CC=avr-gcc
CFLAGS= -mmcu=atmega328p -nostdlib

# ------------------------- CPU frequency and baud rate -----------------------
F_CPU=16000000UL
BAUD= -b 115200

# ------------------------- Conversion .bin -> .hex ---------------------------
HEX=avr-objcopy
HEX_OUT= ihex

# ------------------------- Flasher -------------------------------------------
FLASH=avrdude
PROGRAMMER= -c arduino
CARD= -p atmega328p
PORT= -P /dev/ttyUSB0
FLASH_IN_OUT= -U flash:w:$(NAME).hex:i


# ------------------------- Rules ---------------------------------------------
all: hex flash

hex:
	$(CC) $(CFLAGS) -DF_CPU=$(F_CPU) -O2 -o $(NAME).bin $(NAME).c
	$(HEX) -O $(HEX_OUT) $(NAME).bin $(NAME).hex
	
flash:
	$(FLASH) $(PROGRAMMER) $(CARD) $(PORT) $(BAUD) $(FLASH_IN_OUT)

clean:
	rm -rf $(NAME).bin
	rm -rf $(NAME).hex


.phony: all hex flash clean
