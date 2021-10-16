//#include "sgl_pcd8544.h"
#include "sgl_ili9341.h"
#include "mbed.h"
#include "debug.h"
#include "USBSerial.h"
#include "fonts/Arial11x12.h"

#define RGB565(r,g,b) (((uint16_t)r & 0x00F8) << 8) | (((uint16_t)g & 0x00FC) << 3) | (((uint16_t)b & 0x00F8) >> 3)

// main() runs in its own thread in the OS
int main()
{
    SGLILI9341 lcd(PA_13, PA_14, PA_15, PC_12, PC_11, PC_10);
    lcd.reset();
    lcd.draw_line(11, 11, 200, 200, ILI9341_GREEN);
    lcd.draw_line(11, 12, 200, 201, ILI9341_GREEN);
    lcd.fill_screen(ILI9341_GREEN);
}