/*
 * A set of functons for interacting with the DS3231 RTC module
 * using the HAL library for STM32.
 * 
 * File:   DS3231.c
 * Author: tommy
 *
 * Created on 4 June 2019, 10:17 PM
 */

#include "drivers/DS3231.h"

//Number of days in each month [Jan, Feb, Mar...] of a normal (not leap) year
const int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/*
 * Updates the RTC with a new time and date
 */
void RTC_update_clock(struct Time newTime) {
    //Convert data to a format friendly for the device
	unsigned char newData[7];
    newData[0] = RTC_convert_decimal_to_seconds(newTime.seconds);
    newData[1] = RTC_convert_decimal_to_seconds(newTime.minutes);
    newData[2] = RTC_convert_decimal_to_seconds(newTime.hours);
    newData[3] = dayOfTheWeek(newTime) + 1;
    newData[4] = RTC_convert_decimal_to_seconds(newTime.day);
    newData[5] = RTC_convert_decimal_to_seconds(newTime.month);
    newData[6] = RTC_convert_decimal_to_seconds(newTime.year - 2000);
    
	if(HAL_I2C_IsDeviceReady (&hi2c1, DS3231_ADDR << 1, 10, 250) == HAL_OK) {
			//Write the new time/date data to the clock
			HAL_I2C_Mem_Write (&hi2c1, DS3231_ADDR << 1, DS3231_SECONDS, I2C_MEMADD_SIZE_8BIT, newData, 7, 250);
	}
}

/*
 * Returns a time struct of the current time and date
 */
struct Time RTC_get_time_date() {
    unsigned char timeDate[7];
    struct Time newTime;
    
    //Get the whole chunk of ram as one stream
	if(HAL_I2C_IsDeviceReady (&hi2c1, DS3231_ADDR << 1, 10, 250) == HAL_OK)
		HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDR << 1, DS3231_SECONDS, I2C_MEMADD_SIZE_8BIT, timeDate, 7, 250);
    
    //Have to convert these numbers from the weird HEX format to regular
    //decimal format.
    newTime.seconds = RTC_convert_seconds_to_decimal(timeDate[0]);
    newTime.minutes = RTC_convert_seconds_to_decimal(timeDate[1]);
    newTime.hours = RTC_convert_hours_to_decimal(timeDate[2]);
    newTime.weekday = timeDate[3] - 1;
    newTime.day = RTC_convert_date_to_decimal(timeDate[4]);
    //We can re-use the hours and seconds conversion here.
    newTime.month = RTC_convert_hours_to_decimal(timeDate[5]);
    newTime.year = RTC_convert_seconds_to_decimal(timeDate[6]) + 2000;
    
    return newTime;
}

/*
 * Converts a value from the RTC's werid memory format in to a regular
 * decimal number.
 */
int RTC_convert_seconds_to_decimal(unsigned char data) {
    int tens = ((data & 0xF0) >> 4) * 10;
    int seconds = data & 0x0F;
    
    return tens + seconds;
}
/*
 * Converts the hours register to a decimal value
 */
int RTC_convert_hours_to_decimal(unsigned char data) {
    int tens = ((data & 0x30) >> 4) * 10;
    int digits = data & 0x0F;
    
    return tens + digits;
}
/*
 * Converts the DATE register in to a decimal representation
 */
int RTC_convert_date_to_decimal(unsigned char data) {
    int tens = ((data & 0x30) >> 4) * 10;
    int digits = data & 0x0F;
    
    return tens + digits;
}
/*
 * Converts an integer in to the the format needed for the RTC
 * seconds, minutes, and years register
 */
unsigned char RTC_convert_decimal_to_seconds(int data) {
    int tens = 0;
    //Count the number of tens
    while(data > 9) {
        data -= 10;
        tens++;
    }
    
    //Number of seconds is is whatever is left over
    
    return (tens << 4) | data;
}


/* 
 * Checks that the given Time struct is all within bounds of the
 * Gregorian calendar.
 * Makes sure that there are no negative numbers, no zeros in the date,
 * correct number of days in each month, etc.
 * If a value if incorrect it will be replaced with the nearest correct value.
 */
struct Time checkTimeBounds(struct Time testTime) {
    
    //Check years
    if(testTime.year < 2022)
        testTime.year = 2022;
    //Check month
    if(testTime.month > 12)
        testTime.month = 1;
    if(testTime.month < 1)
        testTime.month = 12;
    //Check days - this is the tricky one
    if(testTime.day > getDaysInMonth(testTime.month, testTime.year))
        testTime.day = 1;
    if(testTime.day < 1)
        testTime.day = getDaysInMonth(testTime.month, testTime.year);
    //Check hours
    if(testTime.hours < 0)
        testTime.hours = 23;
    if(testTime.hours > 23)
        testTime.hours = 0;
    //Check minutes
    if(testTime.minutes < 0)
        testTime.minutes = 59;
    if(testTime.minutes > 59)
        testTime.minutes = 0;
    //Seconds
    if(testTime.seconds < 0)
        testTime.seconds = 59;
    if(testTime.seconds > 59)
        testTime.seconds = 0;
    
    return testTime;
}

/*
 * Calculates the day of the week.
 * Returns the day of the week as an integer beginning at
 * Sunday = 0
 */
int dayOfTheWeek(struct Time thisTime) {
	int adjustment, mm, yy;
    
 
	adjustment = (14 - thisTime.month) / 12;
	mm = thisTime.month + 12 * adjustment - 2;
	yy = thisTime.year - adjustment;
	return (thisTime.day + (13 * mm - 1) / 5 +
		yy + yy / 4 - yy / 100 + yy / 400) % 7;
    
}

/*
 * Returns an array of the number of days in each month for a given year.
 */
int getDaysInMonth(int month, int year) {
    //We count months starting at 1, but the array starts at 0.
    int days = days_in_month[month - 1];
    
    //The special case of the leap year
    /* Rules for leap year:
     * - Divisible by four AND
     * - NOT divisible by 100 OR
     * - Divisible by 400.
     */
    if(month == 2 && ((year%4 == 0 && year%100 != 0) || year%400 == 0)) {
        //Add a day to February
        days = 29;
    }
    
    return days;
}
