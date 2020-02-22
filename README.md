# Clockuino

Simple RTC clock built using Arduino Uno, a DS3231M RTC module and a PCF8574T LCD module.

## Wiring

Wiring is pretty straightforward:

VCC -> 5V
GND -> GND
SCL -> A5
SDA -> A4

## Requirements

 - https://github.com/SV-Zanshin/DS3231M
 - https://github.com/johnrickman/LiquidCrystal_I2C

Install them using the Library Manager in the Arduino IDE.

## Usage

The software accepts two commands:
`RESET` - resets the date/time to the compile date/time
`SET YYYY-MM-DD HH:MM:SS` - sets the date/time to the specified value e.g. `SET 2020-01-22 14:53:12`
