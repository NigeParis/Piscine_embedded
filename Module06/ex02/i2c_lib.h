/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i2c_lib.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 17:30:39 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/27 17:40:07 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef I2C_LIB_H
 #define I2C_LIB_H

    void i2c_init();
    void i2c_start();
    void i2c_stop();
    void printStatus_i2c(void);

    void i2c_write(volatile unsigned char data);



#endif