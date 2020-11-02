//#include "sgl_pcd8544.h"
#include "sgl_ili9341.h"
#include "mbed.h"
#include "debug.h"
#include "USBSerial.h"
//#include "fonts/Arial12x12.h"
#include "fonts/Arial11x12.h"
#include <cstdint>
#include <cstdio>

#define RGB565(r,g,b) (((uint16_t)r & 0x00F8) << 8) | (((uint16_t)g & 0x00FC) << 3) | (((uint16_t)b & 0x00F8) >> 3)

// main() runs in its own thread in the OS
int main()
{   // PinName DC, PinName ce, PinName RST, PinName SPI_MOSI, PinName SPI_MISO, PinName SPI_SCK
    SGLILI9341 disp(PA_14, PA_13, PA_15, PC_12, PC_11, PC_10);
    SGLFont* verd = new Arial11x12Font();
    disp.set_font(verd);

    disp.init();
    disp.fill_screen(ILI9341_YELLOW);
    verd->color = ILI9341_BLACK;
    verd->invert = true;
    disp.draw_string("Hjypgonst char *", 55, 50);
    verd->color = ILI9341_GREENYELLOW;
    disp.draw_string("*ABTypnst char *c", 55, 80);

    verd->color = ILI9341_BLUE;
    disp.draw_string("HyY*%&!", 100, 100);

    disp.draw_char('c', 55, 120);
    verd->color = ILI9341_RED;
    disp.draw_char('%', 55 + 3 + verd->get_char_width('%'), 120);
    while (true)
    {
        ThisThread::sleep_for(chrono::seconds(1));
    }
}