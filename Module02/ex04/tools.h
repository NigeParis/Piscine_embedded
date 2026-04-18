/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 13:56:57 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/18 17:34:27 by nrobinso         ###   ########.fr       */
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

#endif 