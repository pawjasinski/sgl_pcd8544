//#include "sgl_pcd8544.h"
#include "sgl_ili9341.h"
#include "mbed.h"
#include "debug.h"
#include "USBSerial.h"
#include "fonts/Arial12x12.h"
#include "fonts/Verdana11x11b.h"
#include "fonts/Verdana23x20b.h"
#include <cstdint>
#include <cstdio>

#define RGB565(r,g,b) (((uint16_t)r & 0x00F8) << 8) | (((uint16_t)g & 0x00FC) << 3) | (((uint16_t)b & 0x00F8) >> 3)

// Przetestowac, czy CE moze byc zawsze "0" w spi (PIN ce podlaczony do gnd) - sprawdzone, dziala
// sprawdzic wydajnosc przy zapelnianiu calego ekranu - jako pojedyncze piksele oraz jako (addr cale okno)
// wysylanie po koleji send_data, oraz jako funkcji z buforem i liczba do wyslania send_data(command, bufor, dlugosc);
// przetestowac rowniez w ten sam sposob wysylanie interfejsem 16 i 8 bitowym. Rezultaty zapisac i porownac,
// czy sie oplaca modyfikowac kod

// main() runs in its own thread in the OS
int main()
{
    SGLILI9341 display(PA_13, PA_14, PA_15, PC_12, PC_11, PC_10);
    display.init();
    display.fill_screen(0x7BE0);

    Arial12x12Font g;
    display.set_font(static_cast<SGLFont*>(&g));
    display.draw_string("Hello WORLD%", 20, 20);
    Verdana11x11Font v;
    display.set_font(static_cast<SGLFont*>(&v));
    display.draw_string("Hello WORLD%", 20, 35);
    Verdana23x20bFont d;
    display.set_font(static_cast<SGLFont*>(&d));
    display.draw_string("Hello WORLD%", 20, 60, RGB565(100,230,11));
    while (true)
    {
        ThisThread::sleep_for(chrono::seconds(1));
    }
}