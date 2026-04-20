/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   led_lib.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 09:11:22 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/20 14:21:33 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LED_LIB_H
# define LED_LIB_H

    void Blink(uint8_t led);
    void StopBlink(uint8_t led);
    void set_rgb(uint8_t r, uint8_t g, uint8_t b);
    void init_rgb(void);


#endif