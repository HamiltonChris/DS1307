# DS1307 Driver
This is a ds1307 real time clock (RTC) portable driver implementation. This driver supports reading and writing date and time information to and from the ds1307 RTC unit. Additionally it provides functionality to control the square wave output and write to the ds1307 onboard RAM.

## API
- `DS1307_init(DS1307_config* config)`: disables the clock halt bit if set and indicates to the user whether the clock was halted prior to startup.
- `DS1307_set_current_time(DS1307_config* config, RTC_time_t *rtc_time)`: sets the registers for seconds, minutes, hours, and the 12/24 time format.
- `DS1307_get_current_time(DS1307_config* config, RTC_time_t *rtc_time)`: retrieves the value of the time registers and stores them in a `RTC_time_t` struct.
- `DS1307_set_current_date(DS1307_config* config, RTC_date_t *rtc_date)`: sets the registers for the day of the week, date, month, and year.
- `DS1307_get_current_date(DS1307_config* config, RTC_date_t *rtc_date)`: retrieves the value of the date registers and stores them in a `RTC_date_t` struct.
- `DS1307_set_square_wave(DS1307_config* config, RTC_sqwave_t rtc_sqwave)`: configures the square wave output pin, can be configured to output 1Hz, 4kHz, 8kHz, 32kHz and a high or low signal.
- `DS1307_RAM_read(DS1307_config* config, uint8_t reg_addr, uint8_t *data, uint8_t length)`: reads the provided RAM address.
- `DS1307_RAM_write(DS1307_config* config, uint8_t reg_addr, uint8_t *data, uint8_t length)`: writes to the provided RAM address.

## How to use
The configuration struct must be populated with two function pointers that perform the I2C transmit and receive operations with an input I2C device address, input data pointer, and length.
