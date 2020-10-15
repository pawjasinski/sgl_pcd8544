#ifndef __SGL_ST7565R_H__
#define __SGL_ST7565R_H__

#include "mbed.h"
#include "sgl.h"

#define LCD_WIDTH 128 // lcd width
#define LCD_HEIGHT 64 // lcd height

class SGLILI9341 : public SGL
{
    SGLILI9341(PinName DC, PinName CE, PinName RST, PinName BL = NC);
};

#endif // __SGL_ST7565R_H__