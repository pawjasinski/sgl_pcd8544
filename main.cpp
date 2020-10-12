//#include "sgl_pcd8544.h"
#include "sgl_ili9341.h"
#include "mbed.h"
#include "USBSerial.h"

// main() runs in its own thread in the OS
int main()
{
    ThisThread::sleep_for(1s);
    SGLILI9341 display(PA_13, PA_14, PA_15, PC_12, PC_11, PC_10);
    display.spi.format(8, 3);
    display.init();
    display.set_active_window(10, 10, 220, 300);
    display.send_command(ILI9341_RAMWR);
    display.spi.format(16, 3);
    for(int i = 0;i < 220*300;i++) {
        display.spi.write(0x07E0);
    }
    display.ce = 1;
    display.draw_pixel(0, 0, 0x0000);
    display.draw_pixel(1, 1, 0x0000);
    display.draw_pixel(2, 2, 0x0000);
    display.draw_pixel(3, 3, 0x0000);
    display.draw_pixel(4, 4, 0x0000);
}

