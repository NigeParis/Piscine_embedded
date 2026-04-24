/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/24 19:41:25 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "tools.h"
#include "uart_lib.h"
#include "adc_lib.h"

typedef unsigned char uint8_t;      // needed because not using stdlib
volatile char hex[3];               // global for function toHex()
volatile char hex[3];               // global for function toHex()
volatile char hex[3];               // global for function toHex()


void adc_getReading(void(*callback)()) {
    callback();    
}


int main(void) {

    unsigned char buffer = '\0';
    volatile char result[16];
    
    uart_Init();                     // initiate uarts connection
    adc_init(); 
    uart_printstr("connected to the AtMega328P Board ...\r\n");        

    
    while (1) { 
        
        adc_getReading(adc_init_pot);

        buffer = adc_rx();
        toHex(buffer);
    
        result[0] = hex[0];
        result[1] = hex[1];
        result[2] = ',';
        result[3] = ' ';
     
        adc_getReading(adc_init_ldr);

        buffer = adc_rx();
        toHex(buffer);

        result[4] = hex[0];
        result[5] = hex[1];
        result[6] = ',';
        result[7] = ' ';
        
        adc_getReading(adc_init_ntc);
        
        buffer = adc_rx();
        toHex(buffer);

        result[8] = hex[0];
        result[9] = hex[1];
        result[10] = '\r';
        result[11] = '\n';
        result[12] = '\0';
        
        _delay_ms(20);
        uart_printstr(result);
    
    }

}
    
