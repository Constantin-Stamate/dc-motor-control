#include "lcd_driver.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void lcdInit() {
    lcd.init();
    lcd.backlight();
    lcd.print("LCD Ready");
}

void lcdShowStatus(int power) {
    lcd.clear();

    if (power > 0)
        lcd.print("MODE: FWD");
    else if (power < 0)
        lcd.print("MODE: REV");
    else
        lcd.print("MODE: STOP");

    lcd.setCursor(0, 1);
    lcd.print("Power: ");
    lcd.print(abs(power));
    lcd.print("%");
}