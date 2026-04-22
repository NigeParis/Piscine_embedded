/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/22 21:03:42 by nrobinso         ###   ########.fr       */
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

volatile uint8_t brightness = 0;
volatile uint8_t direction = 1;   // 1 = fading in, 0 = fading out



#define LOW 0                                   // PD4 or PD2 pressed
#define LED2_MASK (1 << PB1)                    // PB1 mask select
#define PRESCALER 8                            // SYNIC with CS11 p143 ref: DataSheet
#define TIME_FREQUENCY (F_CPU / PRESCALER) /1000    // MAX Value to be stored in OCR1A
                                                // page 129 on datasheet
                                                // 16 bits = 65565 max
                                                // 16M/8/1 = 2000000 > 65535 too much
                                                // 16M/16/1 = 1000000 > 65535 too much
                                                // 16M/256/1 = 62500 < 65535 OK


void flasher(volatile uint8_t dutycycle) {
    
    // re-calculate dutycycle 10 to 100%
    OCR1A = ((TIME_FREQUENCY) / 100) * dutycycle;
}
                        

   
typedef unsigned char uint8_t;      // needed because not using stdlib

#define LED1 (1 << PB0)
#define SW1 (1 << PD2)

volatile uint8_t last_state = 1;
volatile uint8_t dutycycle = 0.5;

void timer_init_timer1(void) {

    DDRB |= LED2_MASK;
    // set PD2 and PD4 HIGH
    // SET Fast PWM mode 14 - . page 142 
    TCCR1A |= ((1 << COM1A1) | (1 << WGM11)); 
    
    // SET Fast PWM mode 14 NB - TCCR1B timer counter regiister B setting - page 142
    TCCR1B |= ((1 << WGM12) | (1 << WGM13));  
    
    // Set Clk prescaler to F_CPU/256 = 16MHz/256 = 62500 per tick - page 139 & 143 
    TCCR1B |= (1 << CS11);
    
    // ICR1 TOP Value 100% on - 1 sec. - page 129 
    ICR1 = TIME_FREQUENCY - 1;
    
    // send to OCR1A - divide by 10 by %
    // OCR1A = (TIME_FREQUENCY / 1000) * dutycycle;
    OCR1A = 56535; 
    SREG |= (1 << 7);  // global interrupts
}

// void timer_init_timer0(void) {

//     TCCR0A= (1 << COM0B1)   // 8 bit Timer 0 after Schema works on PD6 and PD5 - page 113
//             | (1 << COM0A1) 
//             | (1 << WGM00)  // Mode 3 for timer 0 - WGM00 + WGM01 - page 115 datasheet
//             | (1 << WGM01); // Mode 3 WGM00 + WGM01 --> SAME as WG10 + WGM11

//     TCCR0B= (1 << CS00)     // prescaler 64 - also works with others prescaler setting
//             | (1 << CS01);  // CS01 + C00  - page 117 datasheet
//  }

// /// NOTE: Timer 2 after Schema and page 162 in the datasheet

// void timer_init_timer2(void) {

//     TCCR2A=                 // 8 Bit Timer 2 - page 164 datasheet
//             (1 << COM2B1)   // Only use COM2B1 this works with PD3 - see schema
//             | (1 << WGM21)  // Mode 3 Fast PWM - TOP 0xFF Upd OCRx: BOTTOM Timer Over Flow Flag "TOV" MAX
//             | (1 << WGM20); // PWM mode 3 - page 164 datasheet
            
//     TCCR2B=                
//             (1 << CS22);    // prescaler 64 - also works with others
// }



void __vector_11(void) __attribute__((signal));
void __vector_11(void) {

        flasher(dutycycle);
}





int main(void) {
        
    DDRB |= LED1;    // LED ! - D1 is opened and set on output   
    DDRD &= ~(SW1);    // BUTTON ! - SW1 is opened and set on input   
    PORTD |= SW1;  // set SW1 on pull up;

    timer_init_timer1();

 while(1){
    //  for (int i = 0; i < 100; i++) {
         
    //      dutycycle = dutycycle + 2.5;
    //      __vector_11();
         
    //      _delay_ms(100);
         
    //     }
    //     for (int i = 100; i >= 0; i--) {
            
    //         dutycycle = dutycycle - 2.5;
              
    //         __vector_11();
            
    //         _delay_ms(100);
            
    //     }
    //     dutycycle = 0;
    

    }
    
}

