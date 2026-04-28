/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eeprom_lib.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 14:24:15 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/28 14:32:00 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EEPROM_LIB_H
 #define EEPROM_LIB_H


/// NOTES: EEPROM in the atmega328P has i k of memory - 1024 bytes - page 29 datasheet
/// write/erase cycles 100,000

typedef unsigned char uint8_t;      // needed because not using stdlib
typedef unsigned int uint16_t;      // needed because not using stdlib

 uint8_t eeprom_read(uint16_t address);
 void print_eeprom_hex_line(uint8_t line);

 


#endif