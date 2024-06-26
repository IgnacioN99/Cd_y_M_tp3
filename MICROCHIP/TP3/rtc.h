/*
 * RTC.h
 *
 * Created: 6/6/2024 18:34:12
 * Author : Ignacio
 */ 

#ifndef RTC_H
#define RTC_H
#include "utils.h"

typedef struct {
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t year;
} RTC_Time;

#define I2C_WRITE 0
#define I2C_READ 1

void rtc_init(void);
char *RTC_enviarInformacion();

#endif