#include "sgl_st7565r.h"
#include <chrono>

    /* Start Initial Sequence ----------------------------------------------------
    
    wr_cmd8(0xE2);   //  sw reset
    wait_ms(10);
    
    wr_cmd8(0xAE);   //  display off
    
    wr_cmd8(0xA2);   //  bias voltage (1/9)
  //  wr_cmd8(0xA3);   //  bias voltage (1/7)

    //wr_cmd8(0xA0);   // ADC select seg0-seg131
    wr_cmd8(0xA1);   // ADC select seg223-seg0
    //wr_cmd8(0xC8);   // SHL select com63-com0
    wr_cmd8(0xC0);   // SHL select com0-com63

    wr_cmd8(0x2C);   //   Boost ON
    wait_ms(10);
    wr_cmd8(0x2E);   //   Voltage Regulator ON
    wait_ms(10);
    wr_cmd8(0x2F);   //   Voltage Follower ON
    wait_ms(10);
    wr_cmd8(0x20|0x05);   //  Regulor_Resistor_Select resistor ratio 20-27, look at your display specific init code
    set_contrast(0x20);
    //wr_cmd8(0x81);   //  set contrast (reference voltage register set)
    //wr_cmd8(0x15);   //  contrast 00-3F
    
    wr_cmd8(0xA4);   //  LCD display ram (EntireDisplayOn disable)
    wr_cmd8(0x40);   // start line = 0
    wr_cmd8(0xA6);     // display normal (1 = illuminated)
    wr_cmd8(0xAF);     // display ON 

    --------------------------------------------------------------------------------------------------*/

    /*--------------------------------------------------------------------------------------------------

    if (cs > 0)
    digitalWrite(cs, LOW);

  digitalWrite(rst, LOW);
  _delay_ms(500);
  digitalWrite(rst, HIGH);

  // LCD bias select
  st7565_command(CMD_SET_BIAS_7); // 0xA3
  // ADC select
  st7565_command(CMD_SET_ADC_NORMAL); // 0xA0
  // SHL select
  st7565_command(CMD_SET_COM_NORMAL); // 0xC0
  // Initial display line
  st7565_command(CMD_SET_DISP_START_LINE); // 0x40

  // turn on voltage converter (VC=1, VR=0, VF=0)
  st7565_command(CMD_SET_POWER_CONTROL | 0x4); // 0x28
  // wait for 50% rising
  _delay_ms(50);

  // turn on voltage regulator (VC=1, VR=1, VF=0)
  st7565_command(CMD_SET_POWER_CONTROL | 0x6); // 0x28
  // wait >=50ms
  _delay_ms(50);

  // turn on voltage follower (VC=1, VR=1, VF=1)
  st7565_command(CMD_SET_POWER_CONTROL | 0x7); // 0x28
  // wait
  _delay_ms(10);

  // set lcd operating voltage (regulator resistor, ref voltage resistor)
  st7565_command(CMD_SET_RESISTOR_RATIO | 0x6); // 0x20

    another initial sequence
  -------------------------------------------------------------------------------------------*/

/*--------st7565 pins------------------
/CS     - Chip Select
/RST    - Reset
/A0     - sometimes called RS
/SCLK   - Serial clock
/SID    - Serial Input Data
/VDD    - 3.3V power
---------------------------------------*/

SGLST7565::SGLST7565(PinName A0, PinName CE, PinName RST, PinName SPI_MOSI, PinName SPI_MISO, PinName SPI_SCK)
    : SGL(LCD_WIDTH, LCD_HEIGHT), dc(A0, 1), ce(CE, 1), rst(RST, 1), spi(SPI_MOSI, SPI_MISO, SPI_SCK)
{
    spi.format(8, 3);
    spi.frequency(LCD_SPI_CLOCK);
}

void SGLST7565::init()
{
    reset();
}

void SGLST7565::reset()
{
    send_command(0xE2);   //  sw reset
    ThisThread::sleep_for(chrono::milliseconds(10));
    
    send_command(0xAE);   //  display off
    
    send_command(0xA2);   //  bias voltage (1/9)
  //  wr_cmd8(0xA3);   //  bias voltage (1/7)

    //wr_cmd8(0xA0);   // ADC select seg0-seg131
    send_command(0xA1);   // ADC select seg223-seg0
    //wr_cmd8(0xC8);   // SHL select com63-com0
    send_command(0xC0);   // SHL select com0-com63

    send_command(0x2C);   //   Boost ON
    ThisThread::sleep_for(chrono::milliseconds(10));
    send_command(0x2E);   //   Voltage Regulator ON
    ThisThread::sleep_for(chrono::milliseconds(10));
    send_command(0x2F);   //   Voltage Follower ON
    ThisThread::sleep_for(chrono::milliseconds(10));
    send_command(0x20|0x05);   //  Regulor_Resistor_Select resistor ratio 20-27, look at your display specific init code
    set_contrast(0x20);
    //wr_cmd8(0x81);   //  set contrast (reference voltage register set)
    //wr_cmd8(0x15);   //  contrast 00-3F
    
    send_command(0xA4);   //  LCD display ram (EntireDisplayOn disable)
    send_command(0x40);   // start line = 0
    send_command(0xA6);     // display normal (1 = illuminated)
    send_command(0xAF);     // display ON
}

void SGLST7565::send_data(uint8_t data)
{
    dc.write(1); // stan na high dla przesylania danych
    ce.write(0);
    spi.write(data);
    ce.write(1);
}

void SGLST7565::send_command(uint8_t cmd)
{
    dc.write(0); // ustalamy na low w celu przeslania komendy
    ce.write(0);
    spi.write(cmd);
    ce.write(1);
    //dc.write(1);
}

void SGLST7565::draw_pixel(uint16_t x, uint16_t y, uint16_t color, Mode mode)
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

uint8_t SGLST7565::get_pixel(uint16_t x, uint16_t y)
{
    if ((x < 0) || (x >= LCD_WIDTH) || (y < 0) || (y >= LCD_HEIGHT))
        return 0;

    return (lcd_buffer[x + (y / 8) * LCD_WIDTH] >> (y % 8)) & 0x1;
}

void SGLST7565::display()
{
    dc.write(1); // stan na high dla przesylania danych
    ce.write(0);
    for (unsigned i = 0; i < LCD_BYTES; ++i)
    {
        spi.write(lcd_buffer[i]);
    }
    ce.write(1);
}

void SGLST7565::set_contrast(uint8_t contr)
{
    if (contr > 0x7f)
    {
        contr = 0x7f;
    }
    contrast = contr;
}