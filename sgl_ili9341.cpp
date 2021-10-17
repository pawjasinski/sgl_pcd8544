#include "sgl_ili9341.h"
#include "ThisThread.h"
#include "sgl.h"
#include <chrono>
#include <cstdint>
#include <cstdio>

SGLILI9341::SGLILI9341(PinName DC, PinName ce, PinName RST, PinName SPI_MOSI, PinName SPI_MISO, PinName SPI_SCK)
    : SGL(LCD_WIDTH, LCD_HEIGHT), dc(DC, 1), ce(ce, 0), rst(RST, 1), spi(SPI_MOSI, SPI_MISO, SPI_SCK)
{
    //spi.format(16, 3);
    spi.format(8,3);
    spi.frequency(LCD_SPI_CLOCK);
}

void SGLILI9341::init()
{
    reset();
}
/*
void SGLILI9341::send_data(uint16_t data)
{
    dc.write(1); // stan na high dla przesylania danych
    spi.write(data >> 8);
    spi.write(data&0xff);
}
*/
void SGLILI9341::send_data(uint16_t data)
{
    dc.write(1);
    //ce.write(0);
    spi.write(data >> 8);
    spi.write(data & 0xFF);
    //ce.write(1);
}

void SGLILI9341::send_command(uint8_t cmd)
{
    dc.write(0); // ustalamy na low w //celu przeslania komendy
    //ce.write(0);
    spi.write(cmd);
    //ce.write(1);
}

void SGLILI9341::send_command_parameter(uint8_t param)
{
    dc.write(1);
    //ce.write(0);
    spi.write(param);
    //ce.write(1);
}

void SGLILI9341::draw_pixel(uint16_t x, uint16_t y, uint16_t color, Mode mode)
{
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT)
    {
        return;
    }
    set_active_window(x, y, x, y);

    send_command(ILI9341_RAMWR);
    send_data(color);
}

void SGLILI9341::set_active_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    send_command(ILI9341_CASET); // column address set
    send_data(x0);
    send_data(x1);

    send_command(ILI9341_PASET);
    send_data(y0);
    send_data(y1);

    send_command(ILI9341_RAMWR);
}

void SGLILI9341::fill_screen(uint16_t color)
{
    for(int i = 0; i < 240; ++i)
    {
        for(int j = 0; j < 320; ++j)
        {
            draw_pixel(i, j, color);
        }
    }
}

void SGLILI9341::fill_screen2(uint16_t color)
{
    set_active_window(0, 0, 239, 319);
    for(int i = 0; i < 240*320; ++i)
    {
        send_data(color);
    }
}
/*
void SGLILI9341::draw_horizontal_line(uint16_t x, uint16_t y, int16_t len, uint16_t color, SGL::Mode mode)
{
    if(len == 0) return;
    if(x >= _width)
        x = _width - 1;
    if(y >= _height)
        y = _height - 1;
    int16_t x1 = x + len;
    if(x1 >= _width) x1 = _width - 1;
    if(x1 < 0) x1 = 0;
    if(x1 < x)
    {
        SWAPUINT16(x, x1);
        set_active_window(x, y, x1, y);
        len--;
        dc.write(1);
        while(len++)
            spi.write(color);
    }
    else
    {
        set_active_window(x, y, x1, y);
        len++;
        dc.write(1);
        while(len--)
            spi.write(color);
    }
}
*/
/*
void SGLILI9341::draw_vertical_line(uint16_t x, uint16_t y, int16_t len, uint16_t color, SGL::Mode mode)
{
    if(len == 0) return;
    if(x >= _width) x = _width - 1;
    if(y >= _height) y = _height - 1;
    int16_t y1 = y + len;
    if(y1 >= _height) y1 = _height - 1;
    if(y1 < 0) y1 = 0;
    if(y1 < y)
    {
        SWAPUINT16(y, y1);
        set_active_window(x, y, x, y1);
        len--;
        dc.write(1);
        while(len++)
            spi.write(color);
    }
    else
    {
        set_active_window(x, y, x, y1);
        len++;
        dc.write(1);
        while(len--)
            spi.write(color);
    }
}
*/

void SGLILI9341::set_rotation(uint8_t rot)
{
    ;
}

void SGLILI9341::invert_display(bool invert)
{
    send_command(invert ? ILI9341_INVON : ILI9341_INVOFF);
}

void SGLILI9341::scroll_to(uint16_t h)
{
    send_command(ILI9341_VSCRSADD);
    send_data(h >> 8);
    send_data(h & 255);
}

void SGLILI9341::set_scroll_margins(uint16_t top, uint16_t bottom)
{
    ;
}

void SGLILI9341::reset()
{
    dc = 1;  // dc high
    rst = 0; // display reset
    wait_us(50);
    rst = 1; // end hardware reset
    ThisThread::sleep_for(chrono::milliseconds(5));
    send_command(0x01); // SW reset
    ThisThread::sleep_for(chrono::milliseconds(5));
    send_command(0x28); // display off
    /* Start Initial Sequen//ce ----------------------------------------------------*/
    send_command(0xCF);
    send_command_parameter(0x00);
    send_command_parameter(0x83);
    send_command_parameter(0x30);

    send_command(0xED);
    send_command_parameter(0x64);
    send_command_parameter(0x03);
    send_command_parameter(0x12);
    send_command_parameter(0x81);

    send_command(0xE8);
    send_command_parameter(0x85);
    send_command_parameter(0x01);
    send_command_parameter(0x79);

    send_command(0xCB);
    send_command_parameter(0x39);
    send_command_parameter(0x2C);
    send_command_parameter(0x00);
    send_command_parameter(0x34);
    send_command_parameter(0x02);

    send_command(0xF7);
    send_command_parameter(0x20);

    send_command(0xEA);
    send_command_parameter(0x00);
    send_command_parameter(0x00);


    send_command(0xC0); // POWER_CONTROL_1
    send_command_parameter(0x26);

    send_command(0xC1); // POWER_CONTROL_2
    send_command_parameter(0x11);

    send_command(0xC5); // VCOM_CONTROL_1
    send_command_parameter(0x35);
    send_command_parameter(0x3E);

    send_command(0xC7); // VCOM_CONTROL_2
    send_command_parameter(0xBE);

    send_command(0x36); // MEMORY_AC//ceSS_CONTROL
    send_command_parameter(0x48);

    send_command(0x3A); // COLMOD_PIXEL_FORMAT_SET
    send_command_parameter(0x55);    // 16 bit pixel

    send_command(0xB1); // Frame Rate
    send_command_parameter(0x00);
    send_command_parameter(0x1B);

    send_command(0xF2); // Gamma Function Disable
    send_command_parameter(0x08);

    send_command(0x26);
    send_command_parameter(0x01); // gamma set for curve 01/2/04/08

    send_command(0xE0); // positive gamma correction
    send_command_parameter(0x1F);
    send_command_parameter(0x1A);
    send_command_parameter(0x18);
    send_command_parameter(0x0A);
    send_command_parameter(0x0F);
    send_command_parameter(0x06);
    send_command_parameter(0x45);
    send_command_parameter(0x87);
    send_command_parameter(0x32);
    send_command_parameter(0x0A);
    send_command_parameter(0x07);
    send_command_parameter(0x02);
    send_command_parameter(0x07);
    send_command_parameter(0x05);
    send_command_parameter(0x00);

    send_command(0xE1); // negativ gamma correction
    send_command_parameter(0x00);
    send_command_parameter(0x25);
    send_command_parameter(0x27);
    send_command_parameter(0x05);
    send_command_parameter(0x10);
    send_command_parameter(0x09);
    send_command_parameter(0x3A);
    send_command_parameter(0x78);
    send_command_parameter(0x4D);
    send_command_parameter(0x05);
    send_command_parameter(0x18);
    send_command_parameter(0x0D);
    send_command_parameter(0x38);
    send_command_parameter(0x3A);
    send_command_parameter(0x1F);

    send_command(0xB7); // entry mode
    send_command_parameter(0x07);

    send_command(0xB6); // display function control
    send_command_parameter(0x0A);
    send_command_parameter(0x82);
    send_command_parameter(0x27);
    send_command_parameter(0x00);

    send_command(0x11); // sleep out

    ThisThread::sleep_for(chrono::milliseconds(100));

    send_command(0x29); // display on

    ThisThread::sleep_for(chrono::milliseconds(100));
}

void SGLILI9341::reset2()
{
    rst = 1;
    wait_us(50);
    rst = 0;
    wait_us(20);
    rst = 1;

    send_command(0xC0);
    send_command_parameter(0x19);
    send_command_parameter(0x1a);
    
    send_command(0xC1);
    send_command_parameter(0x45);
    send_command_parameter(0x00);

    send_command(0xC2);
    send_command_parameter(0x33);

    send_command(0xC5);
    send_command_parameter(0x00);
    send_command_parameter(0x28);
    
    send_command(0xB1);
    send_command_parameter(0xA0);
    send_command_parameter(0x11);

    send_command(0xB4);
    send_command_parameter(0x02);

//
//			lcdWriteReg(0xB6);                  //  Display Control Function      
//			lcdWriteData(0x00);
//			lcdWriteDataContinue(0x42);
//			lcdWriteDataContinue(0x3B);
//
//			lcdWriteReg(0xE0);                  //  Positive Gamma control
//			lcdWriteData(0x1F);
//			lcdWriteDataContinue(0x25);
//			lcdWriteDataContinue(0x22);
//			lcdWriteDataContinue(0x0B);
//			lcdWriteDataContinue(0x06);
//			lcdWriteDataContinue(0x0A);
//			lcdWriteDataContinue(0x4E);
//			lcdWriteDataContinue(0xC6);
//			lcdWriteDataContinue(0x39);
//			lcdWriteDataContinue(0x00);
//			lcdWriteDataContinue(0x00);
//			lcdWriteDataContinue(0x00);
//			lcdWriteDataContinue(0x00);
//			lcdWriteDataContinue(0x00);
//			lcdWriteDataContinue(0x00);
//
//			lcdWriteReg(0XE1);                  //  Negative Gamma control
//			lcdWriteData(0x1F);
//			lcdWriteDataContinue(0x3F);
//			lcdWriteDataContinue(0x3F);
//			lcdWriteDataContinue(0x0F);
//			lcdWriteDataContinue(0x1F);
//			lcdWriteDataContinue(0x0F);
//			lcdWriteDataContinue(0x46);
//			lcdWriteDataContinue(0x49);
//			lcdWriteDataContinue(0x31);
//			lcdWriteDataContinue(0x05);
//			lcdWriteDataContinue(0x09);
//			lcdWriteDataContinue(0x03);
//			lcdWriteDataContinue(0x1C);
//			lcdWriteDataContinue(0x1A);
//			lcdWriteDataContinue(0x00);
//
//			//  From original driver, but register numbers don't make any sense.
//			if (0)
//			{
//				lcdWriteReg(0XF1);
//				lcdWriteData(0x36);
//				lcdWriteDataContinue(0x04);
//				lcdWriteDataContinue(0x00);
//				lcdWriteDataContinue(0x3C);
//				lcdWriteDataContinue(0x0F);
//				lcdWriteDataContinue(0x0F);
//				lcdWriteDataContinue(0xA4);
//				lcdWriteDataContinue(0x02);
//
//				lcdWriteReg(0XF2);
//				lcdWriteData(0x18);
//				lcdWriteDataContinue(0xA3);
//				lcdWriteDataContinue(0x12);
//				lcdWriteDataContinue(0x02);
//				lcdWriteDataContinue(0x32);
//				lcdWriteDataContinue(0x12);
//				lcdWriteDataContinue(0xFF);
//				lcdWriteDataContinue(0x32);
//				lcdWriteDataContinue(0x00);
//
//				lcdWriteReg(0XF4);
//				lcdWriteData(0x40);
//				lcdWriteDataContinue(0x00);
//				lcdWriteDataContinue(0x08);
//				lcdWriteDataContinue(0x91);
//				lcdWriteDataContinue(0x04);
//
//				lcdWriteReg(0XF8);
//				lcdWriteData(0x21);
//				lcdWriteDataContinue(0x04);
//			}
//
//			lcdWriteCommand(0x3A, 0x55);
//
//			//  Set initial rotation to match AFX defaults - tall / narrow
//			lcdWriteCommand(0xB6, 0x00, 0x22);
//			lcdWriteCommand(0x36, 0x08);
//
//			lcdWriteReg(0x11); // Sleep out
//
//			//  Fill screen to black
//			writeFillRect2(0, 0, LCD_WIDTH, LCD_HEIGHT, 0x0000);
//
//			lcdWriteReg(0x29);  // Turn on display
//		}
//		endWrite();
//	}
//
}

void SGLILI9341::draw_bitmap16(uint16_t* bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    if(x >= _width)
        x = _width - 1;
    if(y >= _height)
        y = _height - 1;
    if((x + width) >= _width)
        width = _width - x - 1;
    if((y + _height) >= _height)
        _height = _height - y - 1;

    //spi.write16(bitmap, width*height);
    // OR better
    spi.write((char*)bitmap, sizeof(*bitmap)* width * height, NULL, NULL);
}