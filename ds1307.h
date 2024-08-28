/*
 * @file ds1307.h
 *
 * @author Chris Hamilton
 * 
 * @brief driver for ds1307 RTC unit
 */

#ifndef DS1307_H
#define DS1307_H

#include <stdint.h>

#define DS1307_I2C_ADDRESS      0x68

#define DS1307_ADDR_SEC         0x00
#define DS1307_ADDR_MIN         0x01
#define DS1307_ADDR_HRS         0x02
#define DS1307_ADDR_DAY         0x03
#define DS1307_ADDR_DATE        0x04
#define DS1307_ADDR_MONTH       0x05
#define DS1307_ADDR_YEAR        0x06
#define DS1307_ADDR_CONTROL     0x07

#define CLOCK_HALT_BIT  7

#define AM_PM_BIT       5
#define TIME_FORMAT_BIT 6

#define NUM_TIME_REGISTERS  3
#define NUM_DATE_REGISTERS  4
#define NUM_REGISTERS       8

#define TIME_FORMAT_12HRS_AM    0
#define TIME_FORMAT_12HRS_PM    1
#define TIME_FORMAT_24HRS       2

#define SUNDAY                  1
#define MONDAY                  2
#define TUESDAY                 3
#define WEDNESDAY               4
#define THURSDAY                5
#define FRIDAY                  6
#define SATURDAY                7

#define NO_SR   0
#define SR      1

typedef struct RTC_date_s
{
    uint8_t date;
    uint8_t month;
    uint8_t year;
    uint8_t day;
} RTC_date_t;

typedef struct RTC_time_s
{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t time_format;
} RTC_time_t;

typedef enum RTC_sqwave_e
{
    OFF        = 0x00,  // Outputs low signal (0)
    FREQ_1HZ   = 0x10,  // Outputs 1 Hz square wave
    FREQ_4KHZ  = 0x11,  // Outputs 4.096 kHz square wave
    FREQ_8KHZ  = 0x12,  // Outputs 8.192 kHz square wave
    FREQ_32KHZ = 0x13,  // Outputs 32.768 kHz square wave
    HIGH       = 0x80   // Outputs high signal (1)
} RTC_sqwave_t;

typedef struct DS1307_config_s
{
    void (*send)(uint8_t, uint8_t*, uint8_t);
    void (*receive)(uint8_t, uint8_t*, uint8_t);
} DS1307_config;

uint8_t DS1307_init(DS1307_config* config);
void DS1307_set_current_time(DS1307_config* config, RTC_time_t *rtc_time);
void DS1307_get_current_time(DS1307_config* config, RTC_time_t *rtc_time);
void DS1307_set_current_date(DS1307_config* config, RTC_date_t *rtc_date);
void DS1307_get_current_date(DS1307_config* config, RTC_date_t *rtc_date);
void DS1307_set_square_wave(DS1307_config* config, RTC_sqwave_t rtc_sqwave);
void DS1307_RAM_read(DS1307_config* config, uint8_t reg_addr, uint8_t *data, uint8_t length);
void DS1307_RAM_write(DS1307_config* config, uint8_t reg_addr, uint8_t *data, uint8_t length);

#endif 
