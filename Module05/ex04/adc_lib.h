/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adc_lib.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 12:55:04 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/25 12:40:26 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ADC_LIB_H
 #define ADC_LIB_H

    typedef unsigned int uint16_t;      // needed because not using stdlib

    void adc_init(void);
    void adc_select_ldr(void);
    void adc_select_pot(void);
    void adc_select_ntc(void);
    void adc_tx(volatile unsigned char c);
    unsigned char adc_rx(void);
    void adc_init_10_bit(void);
    uint16_t adc_10_bit_rx(void);
    void adc_init_internal_temp_sensor(void);
    void adc_select_temp_sensor_interne(void);


#endif