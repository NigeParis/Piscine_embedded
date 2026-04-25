/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 13:56:57 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/25 09:50:41 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOLS_H
    # define TOOLS_H
    typedef unsigned char uint8_t;      // needed because not using stdlib
    typedef unsigned int uint16_t;      // needed because not using stdlib


    bool is_upper(unsigned char c);
    unsigned char to_upper(unsigned char c); 
    unsigned char to_lower(unsigned char c); 
    unsigned char inverse_char(unsigned char c); 
    bool notPrintable(unsigned char c);
    bool checkChar(unsigned char c);
    int ft_strlen(unsigned char *str);
    bool ft_strcmp(unsigned char *str, unsigned char *name);
    bool is_len_size(unsigned char *str, int size);
    bool is_first_char(unsigned char *str, unsigned char c);
    bool is_valid_hex_str(unsigned char *str);
    void split_hex(unsigned char *str, unsigned char *hex, int start_pos);
    bool is_digit(unsigned char c);
    uint8_t convert_from_hex(unsigned char *str);
    void toHex(unsigned char c);
    void ft_itoa(uint16_t nbr, int index);
    void putnbr(uint16_t nbr);
    uint16_t ft_nbrlen(volatile char *str);
    void nbr_to_str (uint16_t nbr);
    void putnbr(uint16_t nbr);


#endif 