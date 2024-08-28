/*
 * @file ds1307.c
 *
 * @author Chris Hamilton
 * 
 * @brief driver for ds1307 RTC unit
 */

#include "ds1307.h"

static void DS1307_reg_write(const DS1307_config* config,  uint8_t reg_addr, const uint8_t* value, uint8_t length);
static void DS1307_reg_read(const DS1307_config* config, uint8_t reg_addr, uint8_t* output, uint8_t length);
static uint8_t binary_to_bcd(uint8_t binaryValue);
static uint8_t bcd_to_binary(uint8_t bcdValue);


uint8_t DS1307_init(DS1307_config* config)
{
    uint8_t clock_state;
    // Read seconds register
    DS1307_reg_read(config, DS1307_ADDR_SEC, &clock_state, 1);

    // Check if the clock has halted if so reset the bit
    if ((clock_state >> CLOCK_HALT_BIT) & 0x1)
    {
        DS1307_reg_write(config, DS1307_ADDR_SEC, 0x00, 1);
    }

    return ((clock_state >> CLOCK_HALT_BIT) & 0x1); // return whether the clock was halted on startup
}

void DS1307_set_current_time(DS1307_config* config, RTC_time_t *rtc_time)
{
    uint8_t time[NUM_TIME_REGISTERS];

    // seconds
    time[0] = binary_to_bcd(rtc_time->seconds);
    time[0] &= ~(1 << CLOCK_HALT_BIT); // make sure clock halt bit is not set

    // minutes
    time[1] = binary_to_bcd(rtc_time->minutes);

    // hours
    time[2] = binary_to_bcd(rtc_time->hours);
    if (rtc_time->time_format == TIME_FORMAT_24HRS)
    {
        time[2] &= ~(1 << TIME_FORMAT_BIT);
    }
    else
    {
        time[2] |= (1 << TIME_FORMAT_BIT);
        time[2] = (rtc_time->time_format == TIME_FORMAT_12HRS_PM) ? time[2] | (1 << AM_PM_BIT) : time[2] & ~(1 << AM_PM_BIT);
    }
    DS1307_reg_write(config, DS1307_ADDR_SEC, time, NUM_TIME_REGISTERS);
}

void DS1307_get_current_time(DS1307_config* config, RTC_time_t *rtc_time)
{
    uint8_t time[NUM_TIME_REGISTERS];
    DS1307_reg_read(config, DS1307_ADDR_SEC, time, NUM_TIME_REGISTERS);
    time[0] &= ~(1 << 7);   // remove clock halt bit
    rtc_time->seconds = bcd_to_binary(time[0]);
    rtc_time->minutes = bcd_to_binary(time[1]);

    if (time[2] & (1 << TIME_FORMAT_BIT))
    {
        // 12 hours
        rtc_time->time_format = (time[2] & (1 << AM_PM_BIT)) ? TIME_FORMAT_12HRS_PM : TIME_FORMAT_12HRS_AM;
        rtc_time->hours = bcd_to_binary(time[2] & (0x1F));
    }
    else
    {
        // 24 hours
        rtc_time->time_format = TIME_FORMAT_24HRS;
        rtc_time->hours = bcd_to_binary(time[2] & (0x3F));
    }
}

void DS1307_set_current_date(DS1307_config* config, RTC_date_t *rtc_date)
{
    uint8_t date[NUM_DATE_REGISTERS];
    date[0] = binary_to_bcd(rtc_date->day);
    date[1] = binary_to_bcd(rtc_date->date);
    date[2] = binary_to_bcd(rtc_date->month);
    date[3] = binary_to_bcd(rtc_date->year);
    DS1307_reg_write(config, DS1307_ADDR_DAY, date, NUM_DATE_REGISTERS);
}

void DS1307_get_current_date(DS1307_config* config, RTC_date_t *rtc_date)
{
    uint8_t date[NUM_DATE_REGISTERS];
    DS1307_reg_read(config, DS1307_ADDR_DAY, date, NUM_DATE_REGISTERS);
    rtc_date->day = bcd_to_binary(date[0]);
    rtc_date->date = bcd_to_binary(date[1]);
    rtc_date->month = bcd_to_binary(date[2]);
    rtc_date->year = bcd_to_binary(date[3]);
}

void DS1307_set_square_wave(DS1307_config* config, RTC_sqwave_t rtc_sqwave)
{
    uint8_t reg_value = (uint8_t)rtc_sqwave;
    DS1307_reg_write(config, DS1307_ADDR_CONTROL, &reg_value, 1);
}


// @TODO: add error check
void DS1307_RAM_read(DS1307_config* config, uint8_t reg_addr, uint8_t *data, uint8_t length)
{
    // write address to set the register pointer
    config->send(DS1307_I2C_ADDRESS, &reg_addr, 1);

    // receive data from DS1307
    config->receive(DS1307_I2C_ADDRESS, data, length);
}

// @TODO: Fix this function
// @TODO: add error check
void DS1307_RAM_write(DS1307_config* config, uint8_t reg_addr, uint8_t *data, uint8_t length)
{
    // write address to set the register pointer
    config->send(DS1307_I2C_ADDRESS, &reg_addr, 1);
    config->send(DS1307_I2C_ADDRESS, data, length);
}

// @TODO: add error check
static void DS1307_reg_write(const DS1307_config* config,  uint8_t reg_addr, const uint8_t* value, uint8_t length)
{
    uint8_t tx[NUM_REGISTERS];
    tx[0] = reg_addr;
    for (uint8_t i = 0; i < length; i++)
    {
        tx[i + 1] = value[i];
    }
    config->send(DS1307_I2C_ADDRESS, tx, length + 1);
}

// @TODO: add error check
static void DS1307_reg_read(const DS1307_config* config, uint8_t reg_addr, uint8_t* output, uint8_t length)
{
    // write address to set the register pointer
    config->send(DS1307_I2C_ADDRESS, &reg_addr, 1);

    // receive data from DS1307
    config->receive(DS1307_I2C_ADDRESS, output, length);
}

static uint8_t binary_to_bcd(uint8_t binaryValue)
{
    uint8_t bcd = 0;

    bcd |= binaryValue % 10;
    bcd |= ((binaryValue / 10) << 4);

    return bcd;
}

static uint8_t bcd_to_binary(uint8_t bcdValue)
{
    uint8_t binary = 0;

    binary = (bcdValue & 0xF);
    binary += ((bcdValue >> 4) & 0xF) * 10;

    return binary;
}
