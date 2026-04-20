/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 13:51:57 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/20 17:30:48 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools.h"

bool is_upper(char c) {

    if (c >= 'A' && c <= 'Z') {
        return (1);
    }
    return (0);
}

char to_upper(char c) {

    if (c >= 'a' && c <= 'z') {
        c -= 32;   
    }
    return c;
}

char to_lower(char c) {

    if (c >= 'A' && c <= 'Z') {
        c += 32;   
    }
    return c;
}

char inverse_char(char c) {

    if (is_upper(c))
        c = to_lower(c);
    else {
        c = to_upper(c);
    }
    return (c);
}


bool notPrintable(char c) {
    if (c == 0x7F || c == '\r')
        return (0);
    if (c < ' ' || c > '~')
        return (1);
    return (0);
}


bool checkChar(char c) {
    
    if (c == '\t' || c =='\b')
        return (1);
    return (0);
}



int ft_strlen(char *str) {
    
    int i = 0;
    if (!str)
        return (0);
    while (str && str[i])
        i++;
    return (i);
}


bool ft_strcmp(char *str, char *name) {

    int i = 0;

    if (ft_strlen(str) != ft_strlen(name))
        return(0);
    if (str[0] == '\0') 
        return (1);

    while (name[i] != '\0') {

        if (str[i] != name[i])
            return (0);
        i++;
    } 
    
    return (1);
}

/// NOTE: function chhecks the size of the string aginst size
/// ARGS: char* str to be checked, unit8_t size to compare 
/// RETURNS: 0 - false, 1 - true if string is the length found in size 

bool is_len_size(char *str, int size) {

    if (!str)
        return (0);
    if (ft_strlen(str) == size)
        return (1);
    return (0);
}


/// NOTE: function checks first char in a string
/// ARGS: char* str to be checked, char to compare with
/// RETURNS: 0 - false, 1 - true

bool is_first_char(char *str, char c) {

    if (!str)
        return (0);
    if (str && str[0] == c)
        return (1);
    return (0);
}


/// NOTE: function checks the chars are valid HEX in the string
/// ARGS: char* str
/// RETURNS: 0 - false, 1 - true

bool is_valid_hex_str(char *str) {
    
    int i = 1;
    if (!str)
        return (0);

    while (str[i]) {

        if (!((str[i] >= '0' && str[i] <= '9') 
            || (str[i] >= 'a' 
            || str[i] <= 'f')))
                return (0);
        i++;
    }
    return (1);
}


