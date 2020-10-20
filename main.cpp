//#include "sgl_pcd8544.h"
#include "sgl_ili9341.h"
#include "mbed.h"
#include "debug.h"
#include "USBSerial.h"
#include <cstdio>

// Przetestowac, czy CE moze byc zawsze "0" w spi (PIN ce podlaczony do gnd) - sprawdzone, dziala
// sprawdzic wydajnosc przy zapelnianiu calego ekranu - jako pojedyncze piksele oraz jako (addr cale okno)
// wysylanie po koleji send_data, oraz jako funkcji z buforem i liczba do wyslania send_data(command, bufor, dlugosc);
// przetestowac rowniez w ten sam sposob wysylanie interfejsem 16 i 8 bitowym. Rezultaty zapisac i porownac,
// czy sie oplaca modyfikowac kod

// main() runs in its own thread in the OS
int main()
{
    serial_port.write("Begin\n", 6);
    Timer t;
    //serial_port.write("hello\n", 6);
    ThisThread::sleep_for(1s);
    SGLILI9341 display(PA_13, PA_14, PA_15, PC_12, PC_11, PC_10);
    display.init();
    display.fill_screen(ILI9341_GREEN);
    t.start();
    /*
    display.set_active_window(50, 50, 150, 150);
    int h = 10201;
    display.dc = 1;
    while(h--)display.spi.write(ILI9341_GREEN);
    for(int i = 20; i <30; ++i)
        display.draw_horizontal_line(0, i, 239, 0x001F);

    for(int i = 20; i <30; ++i)
        display.draw_vertical_line(i, 0, 319, 0x001F);
    */
    display.draw_string("ABCDEFGHIJK   abcdefghijk", 10, 17, ILI9341_BLACK);
    t.stop();
    int f = t.elapsed_time().count();
    char bff[20];
    //sprintf(bff, "Czas to %d \n", f);
    //serial_port.write(bff, 20);
    while (true)
    {
        ThisThread::sleep_for(chrono::seconds(1));
    }
}