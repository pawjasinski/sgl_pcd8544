#ifndef __SGL_H__
#define __SGL_H__

#include "mbed.h"
#include "font.h"
#include <cstdint>
#include "debug.h"

#define BLACK 0x0000
#define WHITE 0xFFFF

//#define FILL_RECT_HOR
#define FILLL_RECT_VER

#ifndef SWAPINT16
#define SWAPINT16(a, b) \
    {                       \
        int16_t t = a;      \
        a = b;              \
        b = t;              \
    }
#endif
#ifndef SWAPUINT16
#define SWAPUINT16(a, b)\
    {                       \
        uint16_t t = a;     \
        a = b;              \
        b = t;              \
    }
#endif

class SGL // base abstract class for specific drivers
{
public:
    // mode of drawing lcd default is always pixel copy // 0 0 -> 0 ; 1 0 -> 0 ; 0 1 -> 1 ; 1 1 -> 1
    enum class Mode : uint8_t
    {
        pixel_copy = 0x0,   // zwykle nadpisywanie
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
        solid = 0x0, // BLACK for monochrome
        hole = 0x01  // WHITE for monochrome
    };

    SGL(uint16_t w, uint16_t h) : _width(w), _height(h) {}
    virtual ~SGL() {}
    // Podstawowa funkcja do rysowania pixeli
    virtual void draw_pixel(uint16_t x, uint16_t y, uint16_t color = WHITE, Mode mode = Mode::pixel_copy) = 0;
    void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color = WHITE, Mode mode = Mode::pixel_copy);
    // W celu przyspieszenia ryswania na lcd ponizsze funcje powinno sie przeslonic, domyslnie korzystaja z draw_pixel
    // horizontal i vertical line - dla len = 0 brak linii, dla len = 1 pojedynczy pixel
    virtual void draw_horizontal_line(uint16_t x, uint16_t y, int16_t len, uint16_t color = WHITE, Mode mode = Mode::pixel_copy);
    virtual void draw_vertical_line(uint16_t x, uint16_t y, int16_t len, uint16_t color = WHITE, Mode mode = Mode::pixel_copy);
    void draw_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color = WHITE, Fill fill = Fill::hole, Mode mode = Mode::pixel_copy);
    void draw_triangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                       uint16_t color = WHITE, Fill fill = Fill::hole, Mode mode = Mode::pixel_copy);
    void draw_circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t color = WHITE, Fill fill = Fill::hole, Mode mode = Mode::pixel_copy);
    // z fontami to do przemyslenia
    inline void set_font(SGLFont* font) { _font = font; }
    void draw_string(const char* c, uint16_t x, uint16_t y);
    void draw_char(char c, uint16_t x, uint16_t y);
    // na razie zostawiam pusta implementacje
    virtual void draw_bitmap16(uint16_t* bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

protected:

    uint16_t _width;
    uint16_t _height;
    uint16_t x_cursor;
    uint16_t y_cursor;
    SGLFont* _font;
};

#endif // __SGL_H__