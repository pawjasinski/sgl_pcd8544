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
    Timer t, t1;
    char buff[30];
    char buff2[30];

    disp.init();
    t.start();
    disp.fill_screen(ILI9341_YELLOW);
    t.stop();
    sprintf(buff, "yellow fill %d", t.read_us());
    t.reset();
    t.start();
    disp.fill_screen2(ILI9341_GREEN);
    t.stop();
    sprintf(buff2, "green fill %d", t.read_us());
    verd->color = ILI9341_BLACK;
    verd->invert = true;
    disp.draw_string(buff, 100, 150);
    disp.draw_string(buff2, 100, 200);
    disp.draw_horizontal_line(0, 1, 239, ILI9341_BLACK);
    t.reset();
    t.start();
    disp.draw_rectangle(10, 10, 230, 150, ILI9341_BLACK, SGL::Fill::solid);
    t.stop();
    sprintf(buff2, "black box fill %d", t.read_us());
    disp.draw_string(buff2, 60, 250);

    while (true)
    {
        ThisThread::sleep_for(chrono::seconds(1));
    }
}