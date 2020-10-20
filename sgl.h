#ifndef __SGL_H__
#define __SGL_H__

#include "mbed.h"
#include "font.h"
#include <cstdint>

#define BLACK 0x0000
#define WHITE 0xFFFF

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) \
    {                       \
        int16_t t = a;      \
        a = b;              \
        b = t;              \
    }
#define _swap_uint16_t(a, b) \
    {                       \
        uint16_t t = a;      \
        a = b;              \
        b = t;              \
    }
#endif

class SGL
{
public:
    // mode of drawing lcd default is always pixel copy // 0 0 -> 0 ; 1 0 -> 0 ; 0 1 -> 1 ; 1 1 -> 1
    enum class Mode : uint8_t
    {
        pixel_copy = 0x0,
        pixel_or = 0x1,
        pixel_xor = 0x2,
        pixel_clr = 0x3
        //pixel_invt = 0x4,
        //pixel_nor  = 0x5,
        //pixel_xnor = 0x6,
        //pixel_nclr = 0x7
    };

    enum class Fill : uint8_t
    {
        solid = 0x0,
        hole = 0x01
    };

    SGL(uint16_t w, uint16_t h) : _width(w), _height(h) {}
    virtual void draw_pixel(uint16_t x, uint16_t y, uint16_t color = WHITE, Mode mode = Mode::pixel_copy) = 0;
    void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color = WHITE, Mode mode = Mode::pixel_copy);
    // wazne !!!!! w horizontal_line i odpowiednio vertical line len nalezy traktowac jako
    // x+len / y + len, czyli w przypadku len=1 dlugosc linii = 2, dla len = 0 mamy punkt
    // np x0 = 0, y0 = 0, len = 1 wiec x1 = 1, y1 = 1
    virtual void draw_horizontal_line(uint16_t x, uint16_t y, int16_t len, uint16_t color = WHITE, Mode mode = Mode::pixel_copy);
    virtual void draw_vertical_line(uint16_t x, uint16_t y, int16_t len, uint16_t color = WHITE, Mode mode = Mode::pixel_copy);
    void draw_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color = WHITE, Fill fill = Fill::hole, Mode mode = Mode::pixel_copy);
    void draw_triangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                       uint16_t color = WHITE, Fill fill = Fill::hole, Mode mode = Mode::pixel_copy);
    void draw_circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t color = WHITE, Fill fill = Fill::hole, Mode mode = Mode::pixel_copy);
    //void draw_char(char c, uint16_t x, uint16_t y, uint16_t color = WHITE, bool invert = false);
    void set_font(char* font, uint16_t color = WHITE, bool invert = false, bool background = false, uint16_t color_bg = BLACK);
    void draw_char(char c, uint16_t x, uint16_t y, uint16_t color = WHITE);
    void draw_char2(char c, uint16_t x, uint16_t y, uint16_t color = WHITE);
    void draw_string(const char* c, uint16_t x, uint16_t y, uint16_t color = WHITE, bool wrap = true);

protected:
    uint16_t _width;
    uint16_t _height;
    uint16_t x_cursor;
    uint16_t y_cursor;
};

#endif // __SGL_H__