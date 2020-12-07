#include "sgl_ili9341.h"
#include <cstdint>

SGLILI9341::SGLILI9341(PinName DC, PinName ce, PinName RST, PinName SPI_MOSI, PinName SPI_MISO, PinName SPI_SCK)
    : SGL(LCD_WIDTH, LCD_HEIGHT), dc(DC, 1), ce(ce, 0), rst(RST, 1), spi(SPI_MOSI, SPI_MISO, SPI_SCK)
{
    spi.format(16, 3);
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
    spi.write(data);
}

void SGLILI9341::send_command(uint8_t cmd)
{
    dc.write(0); // ustalamy na low w //celu przeslania komendy
    ////ce.write(0);
    spi.write(cmd);
    dc.write(1);
}

void SGLILI9341::send_command_parameter(uint8_t param)
{
    spi.format(8,3);
    dc.write(1);
    spi.write(param);
    spi.format(16,3);
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
    set_active_window(0, 0, _width - 1, _height - 1);
    dc.write(1);
    for(size_t i = _width * _height ; i > 0 ; --i)
    {
        spi.write(color);
    }
}

void SGLILI9341::fill_screen2(uint16_t color)
{
    set_active_window(0, 0, _width - 1, _height - 1);
    dc.write(1);
    spi.write(color, 240*320);
}

void SGLILI9341::draw_horizontal_line(uint16_t x, uint16_t y, int16_t len, uint16_t color, SGL::Mode mode)
{
    if(len == 0) return;
    if(x >= _width) x = _width - 1;
    if(y >= _height) y = _height - 1;
    int16_t x1 = x + len;
    if(x1 >= _width) x1 = _width - 1;
    if(x1 < 0) x1 = 0;
    if(x1 < x)
    {
        _swap_uint16_t(x, x1);
        set_active_window(x, y, x1, y);
        len--;
        dc.write(1);
        //while(len++)
        //spi.write(color);
        len = abs(len);
        spi.write(color, len);
    }
    else
    {
        set_active_window(x, y, x1, y);
        len++;
        dc.write(1);
        //while(len--)
        //spi.write(color);
        spi.write(color,len);
    }
}

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
        _swap_uint16_t(y, y1);
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
    //ce = 1;                           // //ce high
    dc = 1;  // dc high
    rst = 0; // display reset

    wait_us(50);
    rst = 1; // end hardware reset
    //wait_ms(5);
    ThisThread::sleep_for(chrono::milliseconds(5));

    send_command(0x01); // SW reset
    //wait_ms(5);
    ThisThread::sleep_for(chrono::milliseconds(5));
    send_command(0x28); // display off

    /* Start Initial Sequen//ce ----------------------------------------------------*/
    send_command(0xCF);
    send_command_parameter(0x00); // musza buc wyslane 8bit
    send_command_parameter(0x83);
    send_command_parameter(0x30);
    //ce = 1;

    send_command(0xED);
    send_command_parameter(0x64);
    send_command_parameter(0x03);
    send_command_parameter(0x12);
    send_command_parameter(0x81);
    //ce = 1;

    send_command(0xE8);
    send_command_parameter(0x85);
    send_command_parameter(0x01);
    send_command_parameter(0x79);
    //ce = 1;

    send_command(0xCB);
    send_command_parameter(0x39);
    send_command_parameter(0x2C);
    send_command_parameter(0x00);
    send_command_parameter(0x34);
    send_command_parameter(0x02);
    //ce = 1;

    send_command(0xF7);
    send_command_parameter(0x20);
    //ce = 1;

    send_command(0xEA);
    send_command_parameter(0x00);
    send_command_parameter(0x00);
    //ce = 1;

    send_command(0xC0); // POWER_CONTROL_1
    send_command_parameter(0x26);
    //ce = 1;

    send_command(0xC1); // POWER_CONTROL_2
    send_command_parameter(0x11);
    //ce = 1;

    send_command(0xC5); // VCOM_CONTROL_1
    send_command_parameter(0x35);
    send_command_parameter(0x3E);
    //ce = 1;

    send_command(0xC7); // VCOM_CONTROL_2
    send_command_parameter(0xBE);
    //ce = 1;

    send_command(0x36); // MEMORY_AC//ceSS_CONTROL
    send_command_parameter(0x48);
    //ce = 1;

    send_command(0x3A); // COLMOD_PIXEL_FORMAT_SET
    send_command_parameter(0x55);    // 16 bit pixel
    //ce = 1;

    send_command(0xB1); // Frame Rate
    send_command_parameter(0x00);
    send_command_parameter(0x1B);
    //ce = 1;

    send_command(0xF2); // Gamma Function Disable
    send_command_parameter(0x08);
    //ce = 1;

    send_command(0x26);
    send_command_parameter(0x01); // gamma set for curve 01/2/04/08
    //ce = 1;

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
    //ce = 1;

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
    //ce = 1;

    set_active_window(0, 0, _width, _height);

    //send_command(0x34);                     // tearing effect off
    ////ce = 1;

    //send_command(0x35);                     // tearing effect on
    ////ce = 1;
    //ce = 1;

    send_command(0xB7); // entry mode
    send_command_parameter(0x07);
    //ce = 1;

    send_command(0xB6); // display function control
    send_command_parameter(0x0A);
    send_command_parameter(0x82);
    send_command_parameter(0x27);
    send_command_parameter(0x00);
    //ce = 1;

    send_command(0x11); // sleep out
    //ce = 1;

    //wait_ms(100);
    ThisThread::sleep_for(chrono::milliseconds(100));

    send_command(0x29); // display on
    //ce = 1;

    //wait_ms(100);
    ThisThread::sleep_for(chrono::milliseconds(100));
}

void SGLILI9341::reset2()
{
    dc.write(1);
    rst.write(1);
    ThisThread::sleep_for(chrono::milliseconds(1));
    rst.write(0);
    ThisThread::sleep_for(chrono::milliseconds(10));
    rst.write(1);
    ThisThread::sleep_for(chrono::milliseconds(120));

send_command(0xCB); //ILI9341_CMD_POWER_ON_SEQ_CONTROL
send_command_parameter(0x39);
send_command_parameter(0x2C);
send_command_parameter(0x00);
send_command_parameter(0x34);
send_command_parameter(0x02);

send_command(0xCF); //ILI9341_CMD_POWER_CONTROL_B
send_command_parameter(0x00);
send_command_parameter(0xC1);
send_command_parameter(0x30);

send_command(0xE8); //ILI9341_CMD_DRIVER_TIMING_CONTROL_A
send_command_parameter(0x85);
send_command_parameter(0x00);
send_command_parameter(0x78);

send_command(0xEA); //ILI9341_CMD_DRIVER_TIMING_CONTROL_B
send_command_parameter(0x00);
send_command_parameter(0x00);

send_command(0xED);
send_command_parameter(0x64);
send_command_parameter(0x03);
send_command_parameter(0x12);
send_command_parameter(0X81);

send_command(0xF7); //ILI9341_CMD_PUMP_RATIO_CONTROL
send_command_parameter(0x20);

send_command(0xC0); //ILI9341_CMD_POWER_CONTROL_1
send_command_parameter(0x1B);

send_command(0xC1); //ILI9341_CMD_POWER_CONTROL_2
send_command_parameter(0x10);

send_command(0xC5); //ILI9341_CMD_VCOM_CONTROL_1
send_command_parameter(0x2D);
send_command_parameter(0x33);

send_command(0xC7); //ILI9341_CMD_VCOM_CONTROL_2
send_command_parameter(0xCF);

send_command(0x36); //ILI9341_CMD_MEMORY_AC//ceSS_CONTROL
send_command_parameter(0x48);

send_command(0x3A); //ILI9341_CMD_COLMOD_PIXEL_FORMAT_SET
send_command_parameter(0x55);

send_command(0xB1); //ILI9341_CMD_FRAME_RATE_CONTROL_NORMAL
send_command_parameter(0x00);
send_command_parameter(0x1D);

send_command(0xB6); //ILI9341_CMD_DISPLAY_FUNCTION_CONTROL
send_command_parameter(0x08);
send_command_parameter(0x82);
send_command_parameter(0x27);

send_command(0xF2); //ILI9341_CMD_ENABLE_3_GAMMA_CONTROL
send_command_parameter(0x00);

send_command(0x26); //ILI9341_CMD_GAMMA_SET
send_command_parameter(0x1);

send_command(0xE0); //ILI9341_CMD_POSITIVE_GAMMA_CORRECTION
send_command_parameter(0x0F);
send_command_parameter(0x31);
send_command_parameter(0x2B);
send_command_parameter(0x0C);
send_command_parameter(0x0E);
send_command_parameter(0x08);
send_command_parameter(0x4E);
send_command_parameter(0xF1);
send_command_parameter(0x37);
send_command_parameter(0x07);
send_command_parameter(0x10);
send_command_parameter(0x03);
send_command_parameter(0x0E);
send_command_parameter(0x09);
send_command_parameter(0x00);

send_command(0xE1); //ILI9341_CMD_NEGATIVE_GAMMA_CORRECTION
send_command_parameter(0x00);
send_command_parameter(0x0E);
send_command_parameter(0x14);
send_command_parameter(0x03);
send_command_parameter(0x11);
send_command_parameter(0x07);
send_command_parameter(0x31);
send_command_parameter(0xC1);
send_command_parameter(0x48);
send_command_parameter(0x08);
send_command_parameter(0x0F);
send_command_parameter(0x0C);
send_command_parameter(0x31);
send_command_parameter(0x36);
send_command_parameter(0x0F);

send_command(0x11); //ILI9341_CMD_SLEEP_OUT
ThisThread::sleep_for(chrono::milliseconds(120));
send_command(0x29); //ILI9341_CMD_DISPLAY_ON

/*
lcdWriteCommand(FRAME_RATE_CONTROL1);
    lcdWriteParameter(0x0E); // DIVA = 14
    lcdWriteParameter(0x14); // VPA = 20
    
    lcdWriteCommand(DISPLAY_INVERSION);
    lcdWriteParameter(0x07); // NLA = 1, NLB = 1, NLC = 1 (all on Frame Inversion)
   
    lcdWriteCommand(POWER_CONTROL1);
    lcdWriteParameter(0x1F); // VRH = 31:  GVDD = 3,00V
          
    lcdWriteCommand(POWER_CONTROL2);
    lcdWriteParameter(0x00); // BT = 0: AVDD = 2xVCI1, VCL = -1xVCI1, VGH = 4xVCI1, VGL = -3xVCI1

    lcdWriteCommand(VCOM_CONTROL1);
    lcdWriteParameter(0x24); // VMH = 36: VCOMH voltage = 3.4
    lcdWriteParameter(0x64); // VML = 100: VCOML voltage = 0
	
    lcdWriteCommand(VCOM_OFFSET_CONTROL);
    lcdWriteParameter(0x40); // nVM = 0, VMF = 64: VCOMH output = VMH, VCOML output = VML
       	        
	// Set the display to on
    lcdWriteCommand(SET_DISPLAY_ON);
*/

//setOrientation(PORTRAIT); //0x00
//bg(bgcolor); //BLACK
}