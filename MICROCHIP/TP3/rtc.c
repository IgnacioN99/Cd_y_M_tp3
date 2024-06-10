/*
 * rtc.c
 *
 * Created: 6/6/2024 21:18:13
 *  Author: Ignacio
 */ 

#include "rtc.h"
#include "i2c.h"

RTC_Time now = {
	.seconds = 0,
	.minutes = 30,
	.hours = 15,
	.day = 2,  // Día de la semana (1 = Domingo, 7 = Sábado)
	.date = 9,
	.month = 6,
	.year = 24  // Últimos dos dígitos del año
};

void rtc_init() {
	i2c_init(); // Asegura que I2C está inicializado
	rtc_set_time(now);
}

RTC_Time rtc_get_time() {
	RTC_Time time;
	
	i2c_start();
	i2c_write(DS3231_ADDRESS << 1 | I2C_WRITE); // Dirección de DS3231 + bit de escritura
	i2c_write(0x00); // Establecer el registro de segundos como dirección inicial
	i2c_start();
	i2c_write(DS3231_ADDRESS << 1 | I2C_READ); // Dirección de DS3231 + bit de lectura

	time.seconds = bcd_to_dec(i2c_read(1)); // Leer y enviar ACK
	time.minutes = bcd_to_dec(i2c_read(1)); // Leer y enviar ACK
	time.hours = bcd_to_dec(i2c_read(1));   // Leer y enviar ACK
	time.day = bcd_to_dec(i2c_read(1));     // Leer y enviar ACK
	time.date = bcd_to_dec(i2c_read(1));    // Leer y enviar ACK
	time.month = bcd_to_dec(i2c_read(1));   // Leer y enviar ACK
	time.year = bcd_to_dec(i2c_read(0));    // Leer y enviar NACK

	i2c_stop(); // Enviar condición de STOP

	return time;
}

void rtc_set_time(RTC_Time time) {
	i2c_start();
	i2c_write(DS3231_ADDRESS << 1 | I2C_WRITE);
	i2c_write(0x00);  // Establecer el puntero de registro en segundos

	i2c_write(dec_to_bcd(time.seconds));
	i2c_write(dec_to_bcd(time.minutes));
	i2c_write(dec_to_bcd(time.hours));
	i2c_write(dec_to_bcd(time.day));
	i2c_write(dec_to_bcd(time.date));
	i2c_write(dec_to_bcd(time.month));
	i2c_write(dec_to_bcd(time.year % 100));  // Solo dos dígitos para el año

	i2c_stop();
}

uint8_t bcd_to_dec(uint8_t bcd) {
	return ((bcd / 16) * 10) + (bcd % 16);
}

uint8_t dec_to_bcd(uint8_t val) {
	return (val / 10 * 16) + (val % 10);
}