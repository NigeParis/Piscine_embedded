/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/23 12:33:20 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t direction = 0;                     // 1 = fading in, 0 = fading out
volatile uint16_t count = 0;                        // fader counter
volatile uint16_t dutycycle = 0;                    // duty cycle 0 to 100
    
typedef unsigned char uint8_t;                      // needed because not using stdlib
#define CALIBRATE 4000                              // synic flasher in vector 14    
#define LOW 0                                       // PD4 or PD2 pressed
#define LED2_MASK (1 << PB1)                        // PB1 mask select
#define PRESCALER 8                                 // SYNIC with CS11 p143 ref: DataSheet
#define TIME_FREQUENCY (F_CPU / PRESCALER) / 100    // MAX Value to be stored 
                                                    // in OCR1A age 129 on datasheet
                                                    // (16000000 / 8) / 100 = 20000 = OK
                                                    // 16 bits = 65565 max


/// NOTE: function calculates new value for led 
/// using the dutycycle 0% to 100%
/// ARG: new dutycycle to be applied
/// RETURNS: None

void flasher(volatile uint8_t dutycycle) {
    
    OCR1A = ((TIME_FREQUENCY))/100 * dutycycle;
}

///  NOTE: initiate timer 1
                        
void timer_init_timer1(void) {

    TCCR1A |= ((1 << COM1A1) | (1 << WGM11));   // Mode 14 Fast PWM - page 142    
    TCCR1B |= ((1 << WGM12) | (1 << WGM13));      
    TCCR1B |= (1 << CS11) | (1 << CS10);        // prescalier 64 - page 143
    ICR1 = TIME_FREQUENCY - 1;                  // Mode 14 - TOP ICR1 - page 142
    TIMSK1 |= (1 << OCIE1A);                    // enable overflow - fires vector_11 - page 66 / 144
    SREG |= (1 << 7);                           // global interrupts ENABLE
}

///  NOTE: initiate timer 0

void timer_init_timer0(void) {

    TCCR0A= (1 << COM0A0) | (1 << WGM01);       // Mode 2 CTC - TOP OCRA - page 115
    TCCR0B =  (1 << CS12);                      // prescaler 256 - page 117 datasheet
    TIMSK0 |= (1 << OCIE0A);                    // calls __vector_14 - page 66 / 118 
}

/// NOTE: funtion changes the dutycycle form 0% to 100%
/// and changes direction when 100% is reached

void fader(void){
    
    if (dutycycle > 100)
        direction = 1;
    if (dutycycle == 0)
        direction = 0;   
    if (direction == 0)
        dutycycle++;
    if (direction == 1)
        dutycycle--;
}

/// NOTE: vector_14 called by timer 0

void __vector_14(void) __attribute__((signal));
void __vector_14(void) {

    while (count < CALIBRATE)   // small delay to calibrate the led 
        count++;
    count = 0;
    fader();                    // add or subtract 1 from dutycycle in function direction
    TCCR1B |= (1 << CS11);      // start timer 1
    OCR1A= 0;                   // reset value - without the counter stops
}

/// NOTE: vector_11 called by timer 1
 
void __vector_11(void) __attribute__((signal));
void __vector_11(void) {

    TCCR1B &= ~(1 << CS11);  // stop timer 1
    flasher(dutycycle);      // change value in flasher with dutycycle

}


int main(void) {
    DDRB |= LED2_MASK;      // set led PB1 (D2) to output

    timer_init_timer1();    // initate timers
    timer_init_timer0();

 while(1){ }
    
}

