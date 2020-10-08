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
    display.draw_vertival_line(10, 10, -5);
    display.display();
}

