/*
 * I2C.h
 *
 * Created: 6/6/2024 18:34:12
 * Author : Ignacio
 */ 

#ifndef I2C_H
#define I2C_H

#include <avr/io.h>
#include "utils.h"

void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_write(uint8_t data);
uint8_t i2c_read(uint8_t ack);

#endif