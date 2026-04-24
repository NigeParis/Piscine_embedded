/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uart_lib.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 08:24:09 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/24 09:47:17 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UART_LIB_H
# define UART_LIB_H


    void uart_Init(void);
    void uart_interupt_tx(unsigned char c);
    unsigned char uart_interupt_rx(void);
    void uart_printstr(volatile char* string);
    void uart_Init_interupts(void);
    void uart_tx(volatile unsigned char c);
    unsigned char uart_rx(void);

    
#endif