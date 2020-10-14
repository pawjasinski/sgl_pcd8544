#include "sgl_ili9341.h"

SGLILI9341::SGLILI9341(PinName DC, PinName CE, PinName RST, PinName SPI_MOSI, PinName SPI_MISO, PinName SPI_SCK)
                        : SGL(LCD_WIDTH, LCD_HEIGHT), dc(DC, 1), ce(CE, 1), rst(RST, 0), spi(SPI_MOSI, SPI_MISO, SPI_SCK) {
    ;
}

void SGLILI9341::init() {
    spi.frequency(LCD_SPI_CLOCK);
    spi.format(8, 3);
    reset();
}

void SGLILI9341::send_data8(uint16_t data) {
    dc.write(1); // stan na high dla przesylania danych
    ce.write(0);
    //spi.format(8, 3); // odkomentowac i sprawdzic wplyw na wydajnosc - wypelnianie calego ekranu kolorem
    spi.write(data >> 8);
    spi.write(data);
}

void SGLILI9341::send_data16(uint16_t data) {  // sprawdzic o ile wolniejsze (o ile wolniejsze) od send_data8
    dc.write(1); // stan na high dla przesylania danych
    ce.write(0);
    spi.format(16, 3);
    spi.write(data);
    spi.format(8, 3);

    // lub mozna tak, tez przetestowac i sprawdzic wydajnosc, jesli nie bede uzywac 16 bitowego inrefejsu
    // to zakomentowac spi.format(8,3) - dac tylko w konstruktorze
    /*
    spi.format(8,3);
    spi.write(data >> 8);
    spi.write(data);
    */
    
}

void SGLILI9341::send_command8(uint8_t cmd) {
    spi.format(8, 3);
    dc.write(0); // ustalamy na low w celu przeslania komendy
    ce.write(0);
    spi.format(8, 3);
    spi.write(cmd);
    dc.write(1);
}

void SGLILI9341::draw_pixel(uint16_t x, uint16_t y, uint16_t color, Mode mode) {
    if( x >= LCD_WIDTH || y >= LCD_HEIGHT) {
        return;
    }
    set_active_window(x, y, 1, 1);

    send_command(ILI9341_RAMWR);
    send_data8(color);
}

void SGLILI9341::set_active_window(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    send_command8(ILI9341_CASET); // column address set
    send_data16(x);
    //spi.write(x >> 8);
    //spi.write(x);
    send_data16(x + w - 1);
    //spi.write((x + w - 1) >> 8);
    //spi.write(x + w - 1);
    ILI9341_END_WRITE

    send_command8(ILI9341_PASET); // row address 
    send_data16(y);
    //spi.write(y >> 8);
    //spi.write(y);
    send_data16(y + h - 1);
    //spi.write((y + h - 1) >> 8);
    //spi.write(y + h - 1);
    ILI9341_END_WRITE
}

void SGLILI9341::reset()
{
    //ce = 1;                           // ce high
    dc = 1;                           // dc high 
    rst = 0;                        // display reset

    wait_us(50);
    rst = 1;                       // end hardware reset
    //wait_ms(5);
    ThisThread::sleep_for(chrono::milliseconds(5));
     
    send_command(0x01);                     // SW reset  
    //wait_ms(5);
    ThisThread::sleep_for(chrono::milliseconds(5));
    send_command(0x28);                     // display off  

    /* Start Initial Sequence ----------------------------------------------------*/
     send_command(0xCF);                     
     spi.write(0x00);
     spi.write(0x83);
     spi.write(0x30);
     ILI9341_END_WRITE
     
     send_command(0xED);                     
     spi.write(0x64);
     spi.write(0x03);
     spi.write(0x12);
     spi.write(0x81);
     ILI9341_END_WRITE
     
     send_command(0xE8);                     
     spi.write(0x85);
     spi.write(0x01);
     spi.write(0x79);
     ILI9341_END_WRITE
     
     send_command(0xCB);                     
     spi.write(0x39);
     spi.write(0x2C);
     spi.write(0x00);
     spi.write(0x34);
     spi.write(0x02);
     ILI9341_END_WRITE
           
     send_command(0xF7);                     
     spi.write(0x20);
     ILI9341_END_WRITE
           
     send_command(0xEA);                     
     spi.write(0x00);
     spi.write(0x00);
     ILI9341_END_WRITE
     
     send_command(0xC0);                     // POWER_CONTROL_1
     spi.write(0x26);
     ILI9341_END_WRITE
 
     send_command(0xC1);                     // POWER_CONTROL_2
     spi.write(0x11);
     ILI9341_END_WRITE
     
     send_command(0xC5);                     // VCOM_CONTROL_1
     spi.write(0x35);
     spi.write(0x3E);
     ILI9341_END_WRITE
     
     send_command(0xC7);                     // VCOM_CONTROL_2
     spi.write(0xBE);
     ILI9341_END_WRITE
     
     send_command(0x36);                     // MEMORY_ACCESS_CONTROL
     spi.write(0x48);
     ILI9341_END_WRITE
     
     send_command(0x3A);                     // COLMOD_PIXEL_FORMAT_SET
     spi.write(0x55);                 // 16 bit pixel 
     ILI9341_END_WRITE
     
     send_command(0xB1);                     // Frame Rate
     spi.write(0x00);
     spi.write(0x1B);               
     ILI9341_END_WRITE
     
     send_command(0xF2);                     // Gamma Function Disable
     spi.write(0x08);
     ILI9341_END_WRITE
     
     send_command(0x26);                     
     spi.write(0x01);                 // gamma set for curve 01/2/04/08
     ILI9341_END_WRITE
     
     send_command(0xE0);                     // positive gamma correction
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
     ILI9341_END_WRITE
     
     send_command(0xE1);                     // negativ gamma correction
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
     ILI9341_END_WRITE
     
     set_active_window(0, 0, _width, _height);
     
     //send_command(0x34);                     // tearing effect off
     //ce = 1;
     
     //send_command(0x35);                     // tearing effect on
     //ce = 1;
      
     send_command(0xB7);                       // entry mode
     spi.write(0x07);
     ILI9341_END_WRITE
     
     send_command(0xB6);                       // display function control
     spi.write(0x0A);
     spi.write(0x82);
     spi.write(0x27);
     spi.write(0x00);
     ILI9341_END_WRITE
     
     send_command(0x11);                     // sleep out
     ILI9341_END_WRITE
     
     //wait_ms(100);
     ThisThread::sleep_for(chrono::milliseconds(100));
     
     send_command(0x29);                     // display on
     ILI9341_END_WRITE
     
     //wait_ms(100);
     ThisThread::sleep_for(chrono::milliseconds(100));
 }