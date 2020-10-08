#ifndef __SGL_H__
#define __SGL_H__

#include "mbed.h"

#define BLACK 0xFFFF
#define WHITE 0x0000

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

enum class Fill: uint8_t {
    solid = 0x0,
    hole = 0x01
};

    SGL(uint16_t w, uint16_t h): _width(w), _height(h) {}
    virtual void draw_pixel(uint16_t x, uint16_t y, uint16_t color = BLACK, Mode mode = Mode::pixel_copy) = 0;
    void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color = BLACK, Mode mode = Mode::pixel_copy);
    void draw_horizontal_line(uint16_t x, uint16_t y, int16_t len, uint16_t color = BLACK, Mode mode = Mode::pixel_copy);
    void draw_vertival_line(uint16_t x, uint16_t y, int16_t len, uint16_t color = BLACK, Mode mode = Mode::pixel_copy);
    void draw_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color = BLACK, Fill fill = Fill::hole, Mode mode = Mode::pixel_copy);
    void draw_triangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                        uint16_t color = BLACK, Fill fill = Fill::hole, Mode mode = Mode::pixel_copy);

protected:
    uint16_t _width;
    uint16_t _height;
};

#endif // __SGL_H__