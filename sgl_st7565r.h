#ifndef __SGL_ST7565R_H__
#define __SGL_ST7565R_H__

#include "mbed.h"
#include "sgl.h"

#define LCD_WIDTH 128 // lcd width
#define LCD_HEIGHT 64 // lcd height

#define LCD_BYTES 1024 // 128 * 64 / 8

#define LCD_SPI_CLOCK 10000000

class SGLST7565 : public SGL
{
public:
    SGLST7565(PinName A0, PinName CE, PinName RST, PinName SPI_MOSI, PinName SPI_MISO, PinName SPI_SCK);
    void init();
    void reset();
    void send_data(uint8_t data);
    void send_command(uint8_t cmd);
    void draw_pixel(uint16_t x, uint16_t y, uint16_t color = WHITE, Mode mode = Mode::pixel_copy) override;
    uint8_t get_pixel(uint16_t x, uint16_t y); // from the buffer, not from the LCD RAM!!!
    void display();
    
    void set_contrast(uint8_t contr);
    void set_bias(uint8_t b);
    void clear_buffer();
    void inverse_display();
    void normal_display();
    void all_pixel_on_display();
    void all_pixel_off_display();
protected:
    DigitalOut dc; // data/ command // a0
    DigitalOut ce; // or cs, chip select
    DigitalOut rst;// reset

    SPI spi;

    uint8_t lcd_buffer[LCD_BYTES];

    uint8_t contrast;
    uint8_t bias;
};

#endif // __SGL_ST7565R_H__