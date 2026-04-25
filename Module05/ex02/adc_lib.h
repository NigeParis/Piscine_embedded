/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adc_lib.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 12:55:04 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/25 10:09:58 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ADC_LIB_H
 #define ADC_LIB_H

    void adc_init(void);
    void adc_init_ldr(void);
    void adc_init_pot(void);
    void adc_init_ntc(void);
    void adc_tx(volatile unsigned char c);
    unsigned char adc_rx(void);
    void adc_init_10_bit(void);


#endif