#include "sgl_pcd8544.h"

// constructor for hardware SPI
SGLPCD8544::SGLPCD8544(PinName DC, PinName CE, PinName RST, PinName SPI_MOSI, PinName SPI_MISO, PinName SPI_SCK, PinName BL)
    : SGL(LCD_WIDTH, LCD_HEIGHT), dc(DC, 0), ce(CE, 1), rst(RST, 0), bl(BL, 0), spi(SPI_MOSI, SPI_MISO, SPI_SCK)
{
    spi.format(LCD_SPI_BITS, LCD_SPI_MODE);
    spi.frequency(LCD_SPI_CLOCK);
    reset();
}

void SGLPCD8544::init()
{
    normal_display();
    clear_buffer();
    set_bias(0x03);
    set_contrast(60);
}

void SGLPCD8544::reset()
{
    rst.write(0);
    ThisThread::sleep_for(chrono::milliseconds(500));
    rst.write(1);
    normal_display();
}

void SGLPCD8544::backlight(bool onoff)
{
    if (onoff)
    {
        bl.write(1);
    }
    else
    {
        bl.write(0);
    }
}

void SGLPCD8544::send_data(uint8_t data)
{
    dc.write(1); // stan na high dla przesylania danych
    ce.write(0);
    spi.write(data);
    ce.write(1);
}

void SGLPCD8544::send_command(uint8_t cmd)
{
    dc.write(0); // ustalamy na low w celu przeslania komendy
    ce.write(0);
    spi.write(cmd);
    ce.write(1);
    //dc.write(1);
}

void SGLPCD8544::draw_pixel(uint16_t x, uint16_t y, uint16_t color, Mode mode)
{
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT)
    {
        return;
    }
    x %= LCD_WIDTH;
    y %= LCD_HEIGHT;

    if (mode == Mode::pixel_copy)
    {
        mode = color ? Mode::pixel_or : Mode::pixel_clr;
    }

    switch (mode)
    {
    default:
    case Mode::pixel_or:
        lcd_buffer[x + (y / 8) * LCD_WIDTH] |= (1 << (y % 8));
        break;
    case Mode::pixel_xor:
        lcd_buffer[x + (y / 8) * LCD_WIDTH] ^= (1 << (y % 8));
        break;
    case Mode::pixel_clr:
        lcd_buffer[x + (y / 8) * LCD_WIDTH] &= ~(1 << (y % 8));
        break;
    }
}

uint8_t SGLPCD8544::get_pixel(uint16_t x, uint16_t y)
{
    if ((x < 0) || (x >= LCD_WIDTH) || (y < 0) || (y >= LCD_HEIGHT))
        return 0;

    return (lcd_buffer[x + (y / 8) * LCD_WIDTH] >> (y % 8)) & 0x1;
}

void SGLPCD8544::display()
{
    dc.write(1); // stan na high dla przesylania danych
    ce.write(0);
    for (unsigned i = 0; i < LCD_BYTES; ++i)
    {
        spi.write(lcd_buffer[i]);
    }
    ce.write(1);
}

void SGLPCD8544::set_contrast(uint8_t contr)
{
    if (contr > 0x7f)
    {
        contr = 0x7f;
    }
    contrast = contr;

    send_command(LCD_BASICFUNCTION | LCD_EXTENDEDINSTRUCTION);
    send_command(LCD_SETVOP | contr);
    send_command(LCD_BASICFUNCTION);
}

void SGLPCD8544::set_bias(uint8_t b)
{
    if (b > 0x7f)
    {
        b = 0x7f;
    }
    bias = b;

    send_command(LCD_BASICFUNCTION | LCD_EXTENDEDINSTRUCTION);
    send_command(LCD_SETBIAS | b);
    send_command(LCD_BASICFUNCTION);
}

void SGLPCD8544::clear_buffer()
{
    memset(lcd_buffer, 0x00, 504);
}

void SGLPCD8544::inverse_display()
{
    send_command(LCD_BASICFUNCTION);
    send_command(LCD_DISPLAYCONTROL | LCD_DISPLAYINVERTED);
}

void SGLPCD8544::normal_display()
{
    send_command(LCD_BASICFUNCTION);
    send_command(LCD_DISPLAYCONTROL | LCD_DISPLAYNORMAL);
}

void SGLPCD8544::all_pixel_on_display()
{
    send_command(LCD_BASICFUNCTION);
    send_command(LCD_DISPLAYCONTROL | LCD_DISPLAYALLON);
}

void SGLPCD8544::all_pixel_off_display()
{
    send_command(LCD_BASICFUNCTION);
    send_command(LCD_DISPLAYCONTROL | LCD_DISPLAYBLANK);
}
