/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/24 16:56:49 by nrobinso         ###   ########.fr       */
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

int main(void) {

    unsigned char buffer = '\0';
    
    uart_Init();                     // initiate uarts connection
     
    uart_printstr("connected to the AtMega328P Board ...\r\n");        

    adc_init_pot();                  // initiate potentiometre on pin PC0
    
    
    while (1) { 
        
        buffer = adc_rx();
        toHex(buffer);
        uart_tx('\r');
        uart_tx('\n');
        _delay_ms(20);
        adc_tx(hex[0]);
        adc_tx(hex[1]);
    }

}
    
