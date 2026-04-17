/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 13:51:57 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/17 14:09:46 by nrobinso         ###   ########.fr       */
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
