/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/16 09:22:52 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

#define DUTYC 1                                             // Duty cycle 10%
#define TARGET_HZ 1                                         // target 1 hz
#define LED2 (1 << PB1)                                     // PB1 mask select
#define PRESCALER 256                                       // SYNIC with CS12 p142 ref: DataSheet
#define TIME_FREQUENCY ( F_CPU / PRESCALER / TARGET_HZ)     // MAX Value to be stored in OCR1A
                                                            // page 129 on datasheet
                                                            // 16 bits = 65565 max
                                                            // 16M/8/1 = 2000000 > 65535 too much
                                                            // 16M/16/1 = 1000000 > 65535 too much
                                                            // 16M/256/1 = 62500 < 65535 OK
                                                            // how many timer ticks are needed to produce one period of the target frequency
int main(void) {

    // PB1 set for OUTPUT - D2 led
    DDRB |= LED2;

    // SET Fast PWM mode 14 - . page 142 
    TCCR1A |= ((1 << COM1A1) | (1 << WGM11)); 
    
    // SET Fast PWM mode 14 NB - TCCR1B timer counter regiister B setting - page 142
    TCCR1B |= ((1 << WGM12) | (1 << WGM13));  
    
    // Set Clk prescaler to F_CPU/1024 = 16MHz/256 - page 139 & 143 
    TCCR1B |= (1 << CS12);
    
    // ICR1 TOP Value 100% on - 1 sec. - page 129 
    // 16 M / 256 = 62500
    // 62500 - 1 to take account of the 0 -> 62499 = 1 hz per tick
    ICR1 = TIME_FREQUENCY - 1; 
    
    // send to OCR1A - divide by 10 -> 10%
    OCR1A = (ICR1 / 10);

    // % multiplier 0 -> 10 -> 0% to 100 %
    OCR1A = OCR1A * DUTYC;
    
    while(1) {
        
    }
}


/// EXTRA NOTES:
// ===============================
// ATmega328P DATASHEET REFERENCES
// Timer1, Fast PWM, ICR1 as TOP
// ===============================

// 1. Timer/Counter1 Overview
// Datasheet Page 108–109
// - Timer1 is a 16‑bit timer/counter.
// - Supports multiple waveform generation modes (CTC, Fast PWM, Phase Correct).
// - Used when higher resolution or custom TOP values are needed.

// 2. Fast PWM Mode Using ICR1 as TOP
// Datasheet Page 135 (Section 15.9.3)
// - In Fast PWM mode, ICR1 can define the TOP value.
// - Quote from datasheet:
//   "In Fast PWM mode, the counter is incremented until the counter value
//    matches the value in the ICR1 register, and then it is cleared."
// - This means ICR1 directly controls the PWM frequency.

// 3. Frequency Formula (Fast PWM)
// Datasheet Page 136 (Table 15‑5)
// - PWM frequency formula when ICR1 is TOP:
//      f_PWM = f_clk / (Prescaler * (1 + TOP))
// - Since TOP = ICR1, you can compute:
//      ICR1 = (F_CPU / (Prescaler * Target_Frequency)) - 1
// - This is the formula used in your code.

// 4. ICR1 Register Description
// Datasheet Page 142–143
// - ICR1 is the Input Capture Register.
// - In certain WGM modes, ICR1 is used as the TOP value.
// - This allows precise control of PWM frequency or timer overflow rate.

// 5. Prescaler Settings (CS12, CS11, CS10)
// Datasheet Page 131 (Table 15‑6)
// - Clock select bits for Timer1:
//      CS12 CS11 CS10
//      1    0    0   → Prescaler = 256
// - Other prescaler options: 1, 8, 64, 1024, etc.

// ===============================
// Example usage:
// TIME_FREQUENCY = F_CPU / PRESCALER / TARGET_FREQUENCY;
// ICR1 = TIME_FREQUENCY - 1;
// ===============================
