/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/28 09:06:17 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>       // for the function dtostrf()
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

#include "tools.h"
#include "uart_lib.h"
#include "i2c_lib.h"


#define AHT20_ADDRESS 0x38          // address i2c of sensor AHT20 - doc ASAIR page 8

typedef unsigned char uint8_t;      // needed because not using stdlib
typedef unsigned int uint16_t;      // needed because not using stdlib

volatile char hex[3];               // global for function toHex()
volatile char nbr_in_a_string[7];   // global variable for function nbr_to_str()
volatile char pot_reading[5];       // global variable for function nbr_to_str()
volatile uint8_t data_raw[7];               // data raw storage
volatile uint32_t average_raw_temp;               // data raw storage
volatile uint32_t average_raw_humid;               // data raw storage
float tempC;
float humidity;

#define CIM_FREQ_TARGET 100000UL     // 100,000 Khz
#define BIT_RATE_PRESCALER 1    // PAGE 241 
/// TWBR = F_CPU / CIM_FREQ_TARGET) - 16) / (2 * BIT_RATE_PRESCALER
/// CALCULATION SCL - frequency - page 222
///
///                                    CPU Clock frequency
///               SCL frequency = -----------------------
///                             16 + 2(TWBR) * PrescalerValue 

uint8_t count;          // counter for average maker

struct data_input {     //  structure to hold raw data

    uint8_t state;
    uint32_t humidity;
    uint32_t temperature;
    uint8_t crc; 
}; 

struct data_input rawData[3];

/// NOTE: function reads the data from AHT20 senor 7 bytes of data
/// prints it out to screen using uart_printstr
/// ARGS: None
/// RETURNS: None

void i2c_read(void) {
    i2c_write((AHT20_ADDRESS << 1) | 1);                    // read mode  
    for (int i = 0; i < 7; i++) {
        TWCR |= (1 << TWINT) |( 1 << TWEN) | (1 << TWEA);   // page 226 datasheet - import TWEA reads data
        
        while (!(TWCR & (1 << TWINT)))
		        ;
        data_raw[i] = TWDR;        
        if (TW_STATUS != 0x50)
            printStatus_i2c();
    }

    ///NOTE: place raw data instructure[count
    
    rawData[count].state = data_raw[0];
    uint32_t humidity_highend = (uint32_t)data_raw[1] << 16; 
    uint32_t humidity_midend = (uint32_t)data_raw[2] << 8; 
    uint32_t humidity_lowend = (uint32_t)data_raw[3]; 
    rawData[count].humidity = ((humidity_highend | humidity_midend | humidity_lowend) >> 4);    // shift right of 4
    uint32_t highend_nibble = (uint32_t)data_raw[3] & 0x0F;                                     // get four bits xxxxFFFF
    uint32_t temperature_highend = (uint32_t)highend_nibble << 16;
    uint32_t temperature_midend = (uint32_t)data_raw[4] << 8; 
    uint32_t temperature_lowend = (uint32_t)data_raw[5];     
    rawData[count].temperature = ((temperature_highend | temperature_midend | temperature_lowend)); 
    rawData[count].crc = data_raw[6];

    
    
    if (count == 2) {
        
        /// NOTE: formule on page 9 of AHT20 datasheet
        /// calculation of temp and humdity from raw data (average of 3) following the formule
        
        average_raw_temp = (rawData[0].temperature + rawData[1].temperature + rawData[2].temperature) / 3;       
        tempC = ((float)average_raw_temp / 1048576.1f) * 200.1f - 50.1f;
        
        average_raw_humid = (rawData[0].humidity + rawData[1].humidity + rawData[2].humidity) / 3;
        humidity = ((float)average_raw_humid / 1048576.1f) * 100.1f;
        
        char bufferTemp[10];
        dtostrf(tempC, 0, 1, bufferTemp);
        uart_printstr("Temperature: ");     
        uart_printstr(bufferTemp);
        uart_printstr("C,");     
        uart_printstr(" ");    
        
        char bufferHum[10];
        dtostrf(humidity, 0, 1, bufferHum);
        uart_printstr("Humidity: ");     
        uart_printstr(bufferHum);
        uart_printstr("\%");     
        uart_printstr(" ");    
        uart_printstr("\r\n");    
        
        count = 0;                
    }
}


int main(void) {
    
    uart_init();
    i2c_init();
    count  = 0;
    while (1) {
        
        i2c_start();        
        i2c_write((AHT20_ADDRESS << 1) | 0 );   // send AHT20 Sensor Address to start a measurement
        i2c_write(0xAC);                        // start measurements  page 8 datasheet AHT20
        i2c_write(0x33);    
        i2c_write(0x00);
        i2c_stop();

        i2c_start();
        _delay_ms(80);   
        i2c_read();
        i2c_stop();
        count++;        

    }
}

