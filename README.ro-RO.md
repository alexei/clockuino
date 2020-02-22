# Clockuino

Ceas RTC simplu construit cu Arduino Uno, un modul RTC DS3231M și un modul LCD PCF8574T.

## Conectare

Conectarea firelor este destul de simplă:
VCC -> 5V
GND -> GND
SCL -> A5
SDA -> A4

## Biblioteci necesare:

 - https://github.com/SV-Zanshin/DS3231M
 - https://github.com/johnrickman/LiquidCrystal_I2C

Instalează-le folosind Library Manager din Arduino IDE.

## Folosirea programului

Programul acceptă două comenzi:
`RESET` - resetează modulul RTC la data și ora când a fost compilat programul
`SET YYYY-MM-DD HH:MM:SS` - setează data și ora la valorile indicate; ex. `SET 2020-01-22 14:53:12`
