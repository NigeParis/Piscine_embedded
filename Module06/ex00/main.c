/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/26 18:13:35 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/twi.h>
#include "uart_lib.h"

typedef unsigned char uint8_t;      // needed because not using stdlib
typedef unsigned int uint16_t;      // needed because not using stdlib

volatile char hex[3];               // global for function toHex()
volatile char nbr_in_a_string[7];   // global variable for function nbr_to_str()

#define AHT20_ADDRESS 0x38
#define CIM_FREQ_TARGET 100000UL        // 100,000 Khz
#define BIT_RATE_PRESCALER 1            // PAGE 241 

/// TWBR = F_CPU / CIM_FREQ_TARGET) - 16) / (2 * BIT_RATE_PRESCALER = 72
/// CALCULATION SCL - frequency - page 222
///                                    CPU Clock frequency
///               SCL frequency = -----------------------
///                             16 + 2(TWBR) * PrescalerValue 

void printStatus_i2c(void);                    // print status of util/twi.h 

void i2c_init() {

    TWSR &= ~((1 << TWPS0) | (1 << TWPS1));    //  page 241 Prescaler value set to 1 
    TWBR = 72;                                 // set TWBR value for 100,000 Khz- page 241  
}


void i2c_start() {                            //  page - 227 Start the com i2c channel Set TWST(A) - speak

    TWCR |= (1 << TWEN) | (1 << TWSTA) | (1 << TWINT);
    while (!(TWCR &(1<<TWINT)))               // Wait not 0 - page 225
            ;
    printStatus_i2c();                        // output status
  
}


void i2c_stop() {                                       // stop transmission 
    TWCR |= (1 << TWEN) | (1 << TWSTO) | (1 << TWINT);  // Add TWSTO to change to stop
    printStatus_i2c();

}
    

/// NOTE: funcion writes to i2c 
/// ARGS: None
/// RETURNS: None


void i2c_tx(volatile unsigned char c)
{
    // Wait for empty transmit buffer
    TWDR = c;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR &(1 << TWINT)))
    ;
    printStatus_i2c();      
}


int main(void) {
    uart_init();
    
    i2c_init();
    i2c_start();        
    i2c_tx((AHT20_ADDRESS << 1) | 0);    
    i2c_stop();
    
    while (1) { }
}
    


/// NOTE: prints out status of TW_STATUS

void printStatus_i2c(void) {

    switch(TW_STATUS) {
            
        case 0x00:
                uart_printstr("0x00 -illegal start or stop condition\n\r");
                break;
        case 0x08:
                uart_printstr("0x08 -start condition transmitted\n\r");
                break;
        case 0x10:
                uart_printstr("0x10 -repeated start condition transmitted\n\r");
                break;
        case 0x18:
                uart_printstr("0x18 -SLA+W transmitted, ACK received\n\r");
                break;
        case 0x20:
                uart_printstr("0x20 -SLA+W transmitted, NACK received\n\r");
                break;
        case 0x28:
                uart_printstr("0x28 -data transmitted, ACK received\n\r");
                break;
        case 0x30:
                uart_printstr("0x30 -data transmitted, NACK received \n\r");
                break;
        case 0x38:
                uart_printstr("0x38 -arbitration lost in SLA+R or NACK\n\r");
                break;
        case 0x40:
                uart_printstr("0x40 -SLA+R transmitted, ACK received\n\r");
                break;
        case 0x48:
                uart_printstr("0x48 -SLA+R transmitted, NACK received\n\r");
                break;
        case 0x50:
                uart_printstr("0x50 -data received, ACK returned\n\r");
                break;
        case 0x58:
                uart_printstr("0x58 -data received, NACK returned  \n\r");
                break;
        case 0xA8:
                uart_printstr("0xA8 -SLA+R received, ACK returned \n\r");
                break;
        case 0x88:
                uart_printstr("0x88 -data received, NACK returned \n\r");
                break;
        case 0xB0:
                uart_printstr("0xB0 -arbitration lost in SLA+RW, SLA+R received, ACK returned\n\r");
                break;
        case 0xB8:
                uart_printstr("0xB8 -data transmitted, ACK received\n\r");
                break;
        case 0xC0:
                uart_printstr("0xC0 -data transmitted, NACK received\n\r");
                break;
        case 0xC8:
                uart_printstr("0xC8 -last data byte transmitted, ACK received  \n\r");
                break;
        case 0x60:
                uart_printstr("0x60 -SLA+W received, ACK returned\n\r");
                break;
        case 0x68:
                uart_printstr("0x68 -arbitration lost in SLA+RW, SLA+W received, ACK returned\n\r");
                break;
        case 0x70:
                uart_printstr("0x70 -general call received, ACK returned \n\r");
                break;
        case 0x78:
                uart_printstr("0x78 -arbitration lost in SLA+RW, general call received, ACK returned  \n\r");
                break;
        case 0x80:
                uart_printstr("0x80 -data received, ACK returned \n\r");
                break;
        case 0x90:
                uart_printstr("0x90 -general call data received, ACK returned\n\r");
                break;
        case 0x98:
                uart_printstr("0x98 -general call data received, NACK returned\n\r");
                break;
        case 0xA0:
                uart_printstr("0xA0 -stop or repeated start condition received while selected\n\r");
                break;

        case 0xf8:        
                uart_printstr("0xf8 -no state information available\r\n");
                break;
        default : 
               uart_printstr("UNKNOWN\r\n");
    }
}



