#include "sgl_ili9341.h"

SGLILI9341::SGLILI9341(PinName DC, PinName CE, PinName RST, PinName SPI_MOSI, PinName SPI_MISO, PinName SPI_SCK)
    : SGL(LCD_WIDTH, LCD_HEIGHT), dc(DC, 1), ce(CE, 0), rst(RST, 0), spi(SPI_MOSI, SPI_MISO, SPI_SCK)
{
    ;
}

void SGLILI9341::init()
{
    spi.frequency(LCD_SPI_CLOCK);
    spi.format(8, 3);
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
    dc.write(1); // stan na high dla przesylania danych
    //ce.write(0);
    spi.write(data << 8);
    spi.write(data);
}

void SGLILI9341::send_command(uint8_t cmd)
{
    spi.format(8, 3);
    dc.write(0); // ustalamy na low w celu przeslania komendy
    //ce.write(0);
    spi.write(cmd);
    dc.write(1);
}

void send_command_parameter(uint8_t param)
{
    spi.write(param);
}

void SGLILI9341::draw_pixel(uint16_t x, uint16_t y, uint16_t color, Mode mode)
{
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT)
    {
        return;
    }
    set_active_window(x, y, 1, 1);

    send_command(ILI9341_RAMWR);
    send_data(color);
}

void SGLILI9341::set_active_window(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    send_command(ILI9341_CASET); // column address set
    send_data(x);
    send_data(x + w - 1);

    send_command(ILI9341_PASET); // row address
    send_data(y);
    send_data(y + h - 1);
}

void SGLILI9341::reset()
{
    //ce = 1;                           // ce high
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

    /* Start Initial Sequence ----------------------------------------------------*/
    send_command(0xCF);
    spi.write(0x00);
    spi.write(0x83);
    spi.write(0x30);

    send_command(0xED);
    spi.write(0x64);
    spi.write(0x03);
    spi.write(0x12);
    spi.write(0x81);

    send_command(0xE8);
    spi.write(0x85);
    spi.write(0x01);
    spi.write(0x79);

    send_command(0xCB);
    spi.write(0x39);
    spi.write(0x2C);
    spi.write(0x00);
    spi.write(0x34);
    spi.write(0x02);

    send_command(0xF7);
    spi.write(0x20);

    send_command(0xEA);
    spi.write(0x00);
    spi.write(0x00);

    send_command(0xC0); // POWER_CONTROL_1
    spi.write(0x26);

    send_command(0xC1); // POWER_CONTROL_2
    spi.write(0x11);

    send_command(0xC5); // VCOM_CONTROL_1
    spi.write(0x35);
    spi.write(0x3E);

    send_command(0xC7); // VCOM_CONTROL_2
    spi.write(0xBE);

    send_command(0x36); // MEMORY_ACCESS_CONTROL
    spi.write(0x48);

    send_command(0x3A); // COLMOD_PIXEL_FORMAT_SET
    spi.write(0x55);    // 16 bit pixel

    send_command(0xB1); // Frame Rate
    spi.write(0x00);
    spi.write(0x1B);

    send_command(0xF2); // Gamma Function Disable
    spi.write(0x08);

    send_command(0x26);
    spi.write(0x01); // gamma set for curve 01/2/04/08

    send_command(0xE0); // positive gamma correction
    spi.write(0x1F);
    spi.write(0x1A);
    spi.write(0x18);
    spi.write(0x0A);
    spi.write(0x0F);
    spi.write(0x06);
    spi.write(0x45);
    spi.write(0x87);
    spi.write(0x32);
    spi.write(0x0A);
    spi.write(0x07);
    spi.write(0x02);
    spi.write(0x07);
    spi.write(0x05);
    spi.write(0x00);

    send_command(0xE1); // negativ gamma correction
    spi.write(0x00);
    spi.write(0x25);
    spi.write(0x27);
    spi.write(0x05);
    spi.write(0x10);
    spi.write(0x09);
    spi.write(0x3A);
    spi.write(0x78);
    spi.write(0x4D);
    spi.write(0x05);
    spi.write(0x18);
    spi.write(0x0D);
    spi.write(0x38);
    spi.write(0x3A);
    spi.write(0x1F);

    set_active_window(0, 0, _width, _height);

    //send_command(0x34);                     // tearing effect off
    //ce = 1;

    //send_command(0x35);                     // tearing effect on
    //ce = 1;

    send_command(0xB7); // entry mode
    spi.write(0x07);

    send_command(0xB6); // display function control
    spi.write(0x0A);
    spi.write(0x82);
    spi.write(0x27);
    spi.write(0x00);

    send_command(0x11); // sleep out

    //wait_ms(100);
    ThisThread::sleep_for(chrono::milliseconds(100));

    send_command(0x29); // display on

    //wait_ms(100);
    ThisThread::sleep_for(chrono::milliseconds(100));
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

void SGLILI9341::reset2()
{
    dc.write(1);
    rst.write(1);
    ThisThread::sleep_for(chrono::milliseconds(1));
    rst.write(0);
    ThisThread::sleep_for(chrono::milliseconds(10));
    rst.write(1);
    ThisThread::sleep_for(chrono::milliseconds(120));

send_command(ILI9341_CMD_POWER_ON_SEQ_CONTROL); //0xCB
send_command_parameter(0x39);
send_command_parameter(0x2C);
send_command_parameter(0x00);
send_command_parameter(0x34);
send_command_parameter(0x02);

send_command(ILI9341_CMD_POWER_CONTROL_B); //0xCF
send_command_parameter(0x00);
send_command_parameter(0xC1);
send_command_parameter(0x30);

send_command(ILI9341_CMD_DRIVER_TIMING_CONTROL_A); //0xE8
send_command_parameter(0x85);
send_command_parameter(0x00);
send_command_parameter(0x78);

send_command(ILI9341_CMD_DRIVER_TIMING_CONTROL_B); //0xEA
send_command_parameter(0x00);
send_command_parameter(0x00);

send_command(0xED);
send_command_parameter(0x64);
send_command_parameter(0x03);
send_command_parameter(0x12);
send_command_parameter(0X81);

send_command(ILI9341_CMD_PUMP_RATIO_CONTROL); //0xF7
send_command_parameter(0x20);

send_command(ILI9341_CMD_POWER_CONTROL_1); //0xC0
send_command_parameter(0x1B);

send_command(ILI9341_CMD_POWER_CONTROL_2); //0xC1
send_command_parameter(0x10);

send_command(ILI9341_CMD_VCOM_CONTROL_1); //0xC5
send_command_parameter(0x2D);
send_command_parameter(0x33);

send_command(ILI9341_CMD_VCOM_CONTROL_2); //0xC7
send_command_parameter(0xCF);

send_command(ILI9341_CMD_MEMORY_ACCESS_CONTROL); //0x36
send_command_parameter(0x48);

send_command(ILI9341_CMD_COLMOD_PIXEL_FORMAT_SET); //0x3A
send_command_parameter(0x55);

send_command(ILI9341_CMD_FRAME_RATE_CONTROL_NORMAL); //0xB1
send_command_parameter(0x00);
send_command_parameter(0x1D);

send_command(ILI9341_CMD_DISPLAY_FUNCTION_CONTROL); //0xB6
send_command_parameter(0x08);
send_command_parameter(0x82);
send_command_parameter(0x27);

send_command(ILI9341_CMD_ENABLE_3_GAMMA_CONTROL); //0xF2
send_command_parameter(0x00);

send_command(ILI9341_CMD_GAMMA_SET); //0x26
send_command_parameter(0x1);

send_command(ILI9341_CMD_POSITIVE_GAMMA_CORRECTION); //0xE0
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

send_command(ILI9341_CMD_NEGATIVE_GAMMA_CORRECTION); //0xE1
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

send_command(ILI9341_CMD_SLEEP_OUT); //0x11
ThisThread::sleep_for(chrono::milliseconds(120));
send_command(ILI9341_CMD_DISPLAY_ON); //0x29

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