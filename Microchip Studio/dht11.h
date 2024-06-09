#ifndef DHT11_H
#define DHT11_H

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h> 
#include <string.h>
#include <avr/interrupt.h>

char *DHT11_enviarInformacion();

#endif 