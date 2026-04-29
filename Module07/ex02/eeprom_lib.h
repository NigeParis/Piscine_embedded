/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eeprom_lib.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 14:24:15 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/29 17:48:03 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EEPROM_LIB_H
 #define EEPROM_LIB_H


/// NOTES: EEPROM in the atmega328P has i k of memory - 1024 bytes - page 29 datasheet
/// write/erase cycles 100,000

typedef unsigned char uint8_t;      // needed because not using stdlib
typedef unsigned int uint16_t;      // needed because not using stdlib

uint8_t eeprom_read(uint16_t address);
uint16_t print_eeprom_hex_line(uint8_t line, uint16_t address, uint8_t write_flag);
uint16_t print_eeprom_segment(uint16_t line_start, uint16_t line_end, uint16_t line_size);
uint16_t print_eeprom_hexdump_line(uint8_t lineNbr, uint16_t address, uint8_t write_flag);
void print_eeprom_line(uint8_t line);
void eeprom_dispay(int start, int stop, uint16_t address, uint8_t write_flag);
void eeprom_write(uint16_t address, unsigned char data);
uint8_t eeprom_update(uint16_t address, unsigned char data);
uint8_t setEeprom_STATUS(uint8_t eeprom_status);

#endif