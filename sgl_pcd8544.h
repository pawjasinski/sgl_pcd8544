#ifndef __SGL_PCD8544_H_
#define __SGL_PCD8544_H_

/*
RST - linia resetująca rejestry wyświetlacza. Przed rozpoczęciem pracy z wyświetlaczem, należy wygenerować na niej stan zero przez co najmniej 100ns. Podczas normalnej pracy wyświetlacza, na tej linii powinien być ciągle stan wysoki.
CE - jest to linia CS interfejsu SPI, nazwa pochodzi od Chip Enable
DC - linia ustalająca, czy przesyłamy dane (stan wysoki), czy komendy dla wyświetlacza (stan niski)
Din - linia danych, czyli MOSI interfejsu SPI
CLK - linia zegarowa SPI, odpowiada SCLK
VCC - napięcie zasilające moduł (3.3V)
BL - podświetlanie wyświetlacza
GND - masa
*/

#include "mbed.h"
#include "sgl.h"

#define LCD_SPI_BITS 0x08
#define LCD_SPI_MODE 0x00

#define BLACK 0xFFFF         // pixel off
#define WHITE 0x0000         // pixel on
#define FILL 1          // fill
#define NONFILL 0       // non fill

#define LCD_WIDTH 84    // lcd width
#define LCD_HEIGHT 48   // lcd height
#define LCD_BANKS 6     // banks of ram
//#define LCD_BYTES 504   // size of lcd ram

#define LCD_POWERDOWN 0x04              // power down
#define LCD_ENTRYMODE 0x02              // entry mode
#define LCD_EXTENDEDINSTRUCTION 0x01    // extended instruction set
#define LCD_BASICFUNCTION 0x20          // basic instruction set

#define LCD_DISPLAYBLANK 0x0            // blank
#define LCD_DISPLAYNORMAL 0x4           // normal mode
#define LCD_DISPLAYALLON 0x1            // all pixels on
#define LCD_DISPLAYINVERTED 0x5         // all pixels inverted

// Basic instructions
#define LCD_DISPLAYCONTROL 0x08     // set display configuration
#define LCD_SETYADDR 0x40           // set y of ram 0 < y < 5
#define LCD_SETXADDR 0x80           // set x of ram 0 < x < 83

// extended instructions
#define LCD_SETTEMP 0x04        // set lcd tem coefficient dafault 2
#define LCD_SETBIAS 0x10        // set bias // should be 3
#define LCD_SETVOP 0x80         // Write Vop to register

#define LCD_SPI_CLOCK 4000000    // Default to max SPI clock speed for PCD8544 of 4 mhz

#define lcd_bytes = 504

class SGLPCD8544: public SGL {
public:
    //SGLPCD8544(uint8_t CLK, uint8_t DIN, uint8_t DC, uint8_t CE, uint8_t RST = 255, uint8_t BL = 255);
    SGLPCD8544(PinName DC, PinName CE, PinName RST, PinName BL = NC);
    void backlight(bool onoff);
    void init();
    void reset();
    void send_data(uint8_t data);
    void send_command(uint8_t cmd);
    void draw_pixel(uint16_t x, uint16_t y, uint16_t color = BLACK, Mode mode = Mode::pixel_copy) override;
    uint8_t get_pixel(uint8_t x, uint8_t y);
    void display();

    void set_contrast(uint8_t contr);
    void set_bias(uint8_t b);
    void clear_buffer();

protected:
    bool wrapText = true; // zawijaj tekst

    DigitalOut dc;
    DigitalOut ce;
    DigitalOut rst;
    DigitalOut bl;

    uint8_t contrast;
    uint8_t bias;

    uint8_t lcd_buffer[504];

    SPI spi;

};





#endif // __SGL_PCD8544_H_