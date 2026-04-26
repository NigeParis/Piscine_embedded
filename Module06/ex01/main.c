/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/26 17:44:01 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/twi.h>

#include "tools.h"
#include "adc_lib.h"
#include "led_lib.h"
#include "timers.h"
#include "uart_lib.h"

void printStatus_i2c(void);



typedef unsigned char uint8_t;      // needed because not using stdlib
typedef unsigned int uint16_t;      // needed because not using stdlib

volatile char hex[3];               // global for function toHex()
volatile char nbr_in_a_string[7];   // global variable for function nbr_to_str()
volatile char pot_reading[5];   // global variable for function nbr_to_str()

#define CIM_FREQ_TARGET 100000UL     // 100,000 Khz
#define BIT_RATE_PRESCALER 1    // PAGE 241 
/// TWBR = F_CPU / CIM_FREQ_TARGET) - 16) / (2 * BIT_RATE_PRESCALER
/// CALCULATION SCL - frequency - page 222
///                                    CPU Clock frequency
///               SCL frequency = -----------------------
///                             16 + 2(TWBR) * PrescalerValue 


/// NOTE: function set pins D1 to D$ for output
/// ARGS: string label of the pin 'D?' 
/// RETURNS: None
void SetPinOutput(char *str) {

    if (str[0] == 'D') {

        if (str[1] == '1') 
            DDRB |= (1 << PB0);
        if (str[1] == '2') 
            DDRB |= (1 << PB1);
        if (str[1] == '3') 
            DDRB |= (1 << PB2);
        if (str[1] == '4') 
            DDRB |= (1 << PB4);
        if (str[1] == '5') { 
            DDRD |= (1 << PD3);  // BLUE
            DDRD |= (1 << PD5);  // RED
            DDRD |= (1 << PD6);  // GREEN
        }            
    }   
}






void i2c_init() {

    TWSR &= ~((1 << TWPS0) | (1 << TWPS1));         //  page 241 Prescaler value set to 1 
    TWBR = 72;         // set TWBR value for 100,000 Khz- page 241  
    // TWBR |= ((1 << TWD3) | (1 << TWD6));            // set TWDR to 72 - see page 239 add header
    
    // TWCR |= (1 << TWEN);
}


void i2c_start() {

    TWCR |= (1 << TWEN) | (1 << TWSTA) | (1 << TWINT);
    while (!(TWCR &(1<<TWINT)))
            ;
    printStatus_i2c();
  
}


void i2c_stop() {
    TWCR |= (1 << TWEN) | (1 << TWSTO) | (1 << TWINT);
    printStatus_i2c();

}
    


void i2c_tx(volatile unsigned char c)
{
    // Wait for empty transmit buffer
    TWDR = c;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR &(1 << TWINT)))
    ;
    toHex(TW_STATUS);                                       // Put data into buffer, sends the data
    printStatus_i2c();      
}



unsigned char i2c_rx(void) {
    
    unsigned char c;
    while ((TWCR |(1 << TWINT)))
        ;    
    c = TWDR;                                           // receive data
    return (c);                                         // return c
}

#define AHT20_ADDRESS 0x38


int main(void) {
    uart_init();
    
    i2c_init();
    i2c_start();        
    i2c_tx((AHT20_ADDRESS << 1) | 0);    
    i2c_stop();
    
    while (1) { }
}
    





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



