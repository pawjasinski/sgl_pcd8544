//#include "sgl_pcd8544.h"
#include "sgl_ili9341.h"
#include "mbed.h"
#include "USBSerial.h"

// Przetestowac, czy CE moze byc zawsze "0" w spi (PIN ce podlaczony do gnd) - sprawdzone, dziala
// sprawdzic wydajnosc przy zapelnianiu calego ekranu - jako pojedyncze piksele oraz jako (addr cale okno)
// wysylanie po koleji send_data, oraz jako funkcji z buforem i liczba do wyslania send_data(command, bufor, dlugosc);
// przetestowac rowniez w ten sam sposob wysylanie interfejsem 16 i 8 bitowym. Rezultaty zapisac i porownac,
// czy sie oplaca modyfikowac kod

// main() runs in its own thread in the OS
int main()
{
    ThisThread::sleep_for(1s);
    SGLILI9341 display(PA_13, PA_14, PA_15, PC_12, PC_11, PC_10);
    display.init();
    display.ce = 0;
    display.draw_pixel(11, 11, ILI9341_RED);
    display.ce = 0;
    display.draw_pixel(11, 12, ILI9341_RED);
    display.ce = 0;
    display.draw_pixel(11, 13, ILI9341_RED);
    display.ce = 0;
    display.draw_pixel(11, 14, ILI9341_RED);
    display.ce = 0;

    while(true){
        ThisThread::sleep_for(chrono::seconds(1));
    }
}

