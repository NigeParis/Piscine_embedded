/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i2c_lib.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 17:30:36 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/27 17:39:51 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "i2c_lib.h"
#include <util/twi.h>
#include "uart_lib.h"





/// NOTE: program sets up i2c comminication
/// ARGS: None
/// RETURNS: None

void i2c_init() {

    TWSR &= ~((1 << TWPS0) | (1 << TWPS1));         //  page 241 Prescaler value set to 1 
    TWBR = 72;                                      //  see calculation at head of file
}




/// NOTE: starts the i2c coms on - pin 27 and 28  (SCL - clock SDA - data) - page 225 datasheet
/// ARGS: None
/// RETURNS: None

void i2c_start() {

    TWCR |= (1 << TWEN) | (1 << TWSTA) | (1 << TWINT);          // page 226
    while (!(TWCR &(1<<TWINT)))
            ;
        if ((TWSR & 0xF8) != 0x08) {

            uart_printstr("Error: start - condition\r\n");
            printStatus_i2c();
            return ;
        }
  
}




/// NOTE: stop i2c coms
/// TWSTO - page 225 datasheet
/// ARGS: None
/// RETURNS: None

void i2c_stop() {
    TWCR |= (1 << TWEN) | (1 << TWSTO) | (1 << TWINT);
}
    


/// NOTE: write the contents of the TWDR register of the microcontroller

void i2c_write(volatile unsigned char data)
{
    // Wait for empty transmit buffer
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR &(1 << TWINT)))
    ;
}





/// NOTE: code status of avr-libc
/// <util/twi.h>: TWI bit mask definitions
/// https://www.nongnu.org/avr-libc/user-manual/group__util__twi.html

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



