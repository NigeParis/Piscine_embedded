/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 13:56:57 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/20 18:54:51 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOLS_H
    # define TOOLS_H

    bool is_upper(char c);
    char to_upper(char c); 
    char to_lower(char c); 
    char inverse_char(char c); 
    bool notPrintable(char c);
    bool checkChar(char c);
    int ft_strlen(char *str);
    bool ft_strcmp(char *str, char *name);
    bool is_len_size(char *str, int size);
    bool is_first_char(char *str, char c);
    bool is_valid_hex_str(char *str);
    void split_hex(char *str, char *hex, int start_pos);
    bool is_digit(char c);


#endif 