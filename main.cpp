//#include "sgl_pcd8544.h"
#include "sgl_ili9341.h"
#include "mbed.h"
#include "USBSerial.h"

// main() runs in its own thread in the OS
int main()
{
    ThisThread::sleep_for(1s);
    SGLILI9341 display(PA_14, PA_15, PA_13, PC_12, PC_11, PC_10);
    display.init();
    display.draw_pixel(50, 50, 0xF800);
    display.draw_pixel(51, 50, 0xF800);
    display.draw_pixel(50, 51, 0xF800);
    display.draw_pixel(51, 51, 0xF800);
    display.set_active_window(10, 10, 20, 20);

    display.send_command(ILI9341_CASET);
    display.spi.write(10 >> 8);
    display.spi.write(10);
    display.spi.write(29 >> 8);
    display.spi.write(29);
    display.ce = 1;
    display.send_command(ILI9341_PASET);
    display.spi.write(10 >> 8);
    display.spi.write(10);
    display.spi.write(29 >> 8);
    display.spi.write(29);
    display.ce = 1; // koniec komendy / wysylanie danych na ten moment!!!!!
    display.send_command(ILI9341_RAMWR);
    display.spi.format(16, 3);
    for(int i = 0;i < 100;i++)display.spi.write(0xF800);
    display.ce = 1;
}

