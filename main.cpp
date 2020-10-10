#include "sgl_pcd8544.h"
#include "mbed.h"
#include "USBSerial.h"

// main() runs in its own thread in the OS
int main()
{
    SGLPCD8544 display(PA_13, PA_14, PA_15, PB_7);
    ThisThread::sleep_for(1s);
    display.backlight(true);
    ThisThread::sleep_for(500);
    display.backlight(false);
    ThisThread::sleep_for(500);
    display.backlight(true);
    //display.draw_rectangle(10, 10, 0, 0);
    //display.draw_rectangle(33, 47, 11, 11, BLACK, SGL::Fill::solid);
    display.draw_circle(50, 20, 10, BLACK, SGL::Fill::solid);
    //display.draw_triangle(1, 1, 60, 2, 40, 40, BLACK, SGL::Fill::hole);
    display.display();
    while(true) {
        ThisThread::sleep_for(1s);
    }
}

