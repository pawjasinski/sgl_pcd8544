#ifndef __SGL_H__
#define __SGL_H__

#include "mbed.h"

class SGL{
public:
    // mode of drawing lcd default is always pixel copy // 0 0 -> 0 ; 1 0 -> 0 ; 0 1 -> 1 ; 1 1 -> 1
    enum class Mode : uint8_t {
        pixel_copy = 0x0,
        pixel_or   = 0x1,
        pixel_xor  = 0x2,
        pixel_clr  = 0x3
        //pixel_invt = 0x4,
        //pixel_nor  = 0x5,
        //pixel_xnor = 0x6,
        //pixel_nclr = 0x7
};

    SGL(uint8_t w, uint8_t h): _width(w), _height(h) {}
    virtual void draw_pixel(uint8_t x, uint8_t y, uint8_t color, Mode mode = Mode::pixel_copy) = 0;
    void draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color = 1, Mode mode = Mode::pixel_copy);
    void draw_line_test(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color = 1, Mode mode = Mode::pixel_copy);

protected:
    uint8_t _width;
    uint8_t _height;
};

#endif // __SGL_H__