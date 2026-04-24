/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/23 20:34:40 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

#define LED1_MASK (1 << PB0)
#define LED2_MASK (1 << PB1)
#define LED3_MASK (1 << PB2)
#define LED4_MASK (1 << PB4)
#define LED_MASK  (LED1_MASK | LED2_MASK | LED3_MASK | LED4_MASK )
#define SW1_MASK  (1 << PD2)
#define SW2_MASK  (1 << PD4)
#define BUTTON_MASK (SW1_MASK | SW1_MASK)
#define LOW 0

volatile uint8_t buttonFlag = 0;
volatile uint8_t ledFlag = 0;
volatile uint8_t count = 0;
volatile uint16_t loop = 0;





void ledNumber(uint8_t count) {
    
    uint8_t bit3 = count &(0b00001000);             // isolate bit 3 of count
    uint8_t bit4 = bit3 << 1;                       // move it to bit 4 (PB4) 0b1000 -> 0b10000
    uint8_t first_three_bits = count &(0b00000111); // bits 0–2 of count
    uint8_t val = first_three_bits | bit4;          // combine into LED pattern
    uint8_t cleared_led_bits = PORTB & ~LED_MASK;   // clear LED bits only
    PORTB = cleared_led_bits | (val &(0b00010111));                 // write new LED state
    if (((cleared_led_bits &(0b00010111))) == 0) {count = 0;}  
}


void timer_init_timer0A(void) {

    // TCCR0A= (1 << COM0A0) | (1 << WGM01);       // Mode 2 CTC - TOP OCRA - page 115
    // TCCR0B =  (1 << CS12);                      // prescaler 256 - page 117 datasheet
    // EICRA |= (1 << ISC01);                      
    EIMSK |= (1 << INT0);                    // calls __vector_1 - page 66 / 118 
    SREG |= (1 << 7);                           // global interrupts ENABLE
    
}



void timer_init_timer0B(void) {

    // TCCR0A= (1 << COM0B1) | (1 << WGM01);       // Mode 2 CTC - TOP OCRA - page 115
    // TCCR0B =  (1 << CS02);                      // prescaler 256 - page 117 datasheet
    TIMSK0 |= (1 << OCIE0B);
    TIFR0 |= (1 << OCF0B);                      


}



void timer_init_timer2B(void) {


    // TCCR2A= (1 << COM2B0) | (1 << WGM21);       // Mode 2 CTC - TOP OCRA - page 115
    // TCCR2B = (1 << CS21);// | (1 << CS22));                      // prescaler 256 - page 117 datasheet
    PCICR |= (1 << PCIE2);                    // calls __vector_?? - page 82
    PCMSK2 |= (1 << PCINT20);                   // page 83 
    SREG |= (1 << 7);                           // global interrupts ENABLE
    
    
}




/// NOTE: vector_1 called by timer 0A

void __vector_5(void) __attribute__((signal));
void __vector_5(void) {

    if (!(PIND & (1 << PD4))){   
        _delay_ms(20);
        if (!(PIND & (1 << PD4))){
            if (count != 0) {
                count--;
                ledNumber(count);
            }
        }
    }
}

/// NOTE: vector_5 called by timer 0B

void __vector_1(void) __attribute__((signal, used));
void __vector_1(void) {

    if (!(PIND & (1 << PD2))){   
        _delay_ms(20);
        if (!(PIND & (1 << PD2))){
            
            count++;
            if (count > 15)
            count = 15;    
        ledNumber(count);
        }
    } 
}











int main(void) {

    
    timer_init_timer0A();
    // timer_init_timer0B();
    timer_init_timer2B();
    DDRB |= LED_MASK;
    PORTB &= ~LED_MASK;
    DDRD &= ~BUTTON_MASK;
    PORTD |= BUTTON_MASK;  //PORTD - page 72
   

    while (1) { }

}

    