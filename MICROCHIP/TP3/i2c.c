/*
 * i2c.c
 *
 * Created: 6/6/2024 18:34:12
 * Author : Ignacio
 */ 

#include "i2c.h"

void i2c_init() {
	// Configurar el registro TWBR para la frecuencia SCL
	TWBR = (uint8_t)(((F_CPU / SCL_CLOCK) - 16) / 2);
	TWSR = 0; // Preescaler a 1
}

void i2c_start() {
	// Enviar condici�n de START
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT))); // Esperar a que se complete
}

void i2c_stop() {
	// Enviar condici�n de STOP
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void i2c_write(uint8_t data) {
	TWDR = data; // Cargar dato en el registro TWDR
	TWCR = (1 << TWINT) | (1 << TWEN); // Iniciar transmisi�n
	while (!(TWCR & (1 << TWINT))); // Esperar a que se complete
}

uint8_t i2c_read(uint8_t ack) {
	TWCR = (1 << TWINT) | (1 << TWEN) | (ack << TWEA); // Configurar ACK
	while (!(TWCR & (1 << TWINT))); // Esperar a que se complete la recepci�n
	return TWDR; // Devolver el dato recibido
}