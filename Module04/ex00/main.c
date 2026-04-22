/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/22 19:33:13 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <util/delay.h>                         // delay for the debounce
#include <avr/io.h>
#include <avr/interrupt.h>
// #include "tools.h"
// #include "uart_lib.h"
// #include "config.h"
// #include "led_lib.h"
// #include "timers.h"

typedef unsigned char uint8_t;      // needed because not using stdlib

#define LED1 (1 << PB0)
#define SW1 (1 << PD2)

volatile uint8_t last_state = 1;

void timer_init_timer1() {
    
    /// NOTE:  timer 1 is in 16 bits   
    /// CALCULE: ((F_CPU / PRESCAILER) *
    /// RESULT:  (16 000 000 / prescailer 64) * target (20 ms) 0.02 = 5000 (16 bits need to fit)
    /// REF: Module02/ex01 TIME_FREQUENCY = (F_CPU / PRESCALER) / TARGET_HZ  
    /// REF: value to be stored in TCNT1 (16bits) - 
    // TCNT1 = 0;
    
    SREG |= (1 << 7);  // ENABLE Global interupts page 20
    TCCR1A &= ~((1 << COM1A1) | (1 << COM1B1));
    TCCR1B |= (1 << CS11) | (1 << CS10);             // prescaler 64 - page 143
    TIMSK1 |= (1 << OCIE1A);                          // enable overflow - fires vector_11
    // TIFR1 |= (1 << OCF1A);
    // TCNT1 = 50100;
    
}

void led_sw_init(void ) { 
    
    EIMSK = (1 << INT0);  // page 81 activate intyrupt INTO_vect _vector_1 call
    EICRA |= (1 << ISC01);  // trigger on falling edge
}    

void __vector_11(void) __attribute__((signal)); // TIMER1_vect interupt 1 - page 66
void __vector_11(void)
{
    
    TCCR1B &= ~((1 << CS11) | (1 << CS10));    // prescaler 64 page 142

    if (last_state == 2 ) {
        PORTB |= LED1;
        last_state = 1;
    } else {
        PORTB &= ~(LED1);
        last_state = 2;
    }    
    DDRD &= ~(SW1);    // BUTTON ! - SW1 is opened and set on input   
}

void __vector_1(void) __attribute__((signal)); // INTO_vect interupt 1 - page 66
void __vector_1(void) {
    
    PORTB &= ~LED1;
    DDRD |= (SW1);      // BUTTON ! - SW1 is opened and set on input   
    TCCR1B |= (1 << CS11) | (1 << CS10);  // prescaler 64 - start counter
    TCNT1 = 50100;      // 20 ms = 5000 - see calculation at the top - start time for counter 1
    
}

int main(void) {
        
    DDRB |= LED1;    // LED ! - D1 is opened and set on output   
    DDRD &= ~(SW1);    // BUTTON ! - SW1 is opened and set on input   
    PORTD |= SW1;  // set SW1 on pull up;

    timer_init_timer1();
    led_sw_init();    
    while(1){ }
    
}

