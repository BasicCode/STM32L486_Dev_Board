/* 
 * File:   DS3231.h
 * Author: tommy
 *
 * Created on 4 June 2019, 10:17 PM
 */

#ifndef DS3231_H
#define	DS3231_H

#include "stm32l4xx_hal.h"

//Chose your I2C port here
extern I2C_HandleTypeDef hi2c1;


//List of days of the week
static const char dayName[][10] = {"Sunday", "Monday", "Tuesday", "Wednesday",
		"Thursday", "Friday", "Saturday"};

//A time structure
struct Time {
	unsigned char seconds;
	unsigned char minutes;
	unsigned char hours;
	unsigned char weekday;
	unsigned char day;
	unsigned char month;
	unsigned int year;
};

//The device address
#define DS3231_ADDR 0x68

//Device memory addresses
#define DS3231_SECONDS      0x00
#define DS3231_MINUTES      0x01
#define DS3231_HOURS        0x02
#define DS3231_WEEKDAY      0x03 //Day of the week (1-7)
#define DS3231_DAYS         0x04 //Day of the month (1 - 31)
#define DS3231_MONTHS       0x05 //Month of the year (1 - 12)
#define DS3231_YEARS        0x06
#define DS3231_CONTROL_1    0x0E //Control bits register
#define DS3231_CONTROL_2    0x0F //Status bits register
#define DS3231_TEMP_H       0x11 //Temeprature registers
#define DS3231_TEMP_L       0x12


void RTC_update_clock(struct Time newTime);
struct Time RTC_get_time_date();
int RTC_convert_seconds_to_decimal(unsigned char data);
int RTC_convert_hours_to_decimal(unsigned char data);
int RTC_convert_date_to_decimal(unsigned char data);
unsigned char RTC_convert_decimal_to_seconds(int data);
unsigned char RTC_convert_decimal_to_hours(int data);
unsigned char RTC_convert_decimal_to_date(int data);
struct Time checkTimeBounds(struct Time testTime);
int dayOfTheWeek(struct Time thisTime);
int getDaysInMonth(int month, int year);

#endif	/* DS3231_H */

