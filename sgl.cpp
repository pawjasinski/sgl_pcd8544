#include "sgl.h"
#include "Callback.h"
#include "debug.h"
#include <cstdint>
#include <cstring>

void SGL::draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color, Mode mode)
{
    // check if parameters are in range
    if (x0 >= _width)
        x0 = _width - 1;
    if (x1 >= _width)
        x1 = _width - 1;
    if (y0 >= _height)
        y0 = _height - 1;
    if (y1 >= _height)
        y1 = _height - 1;

    int16_t dx = abs(x1 - x0);
    int16_t dy = abs(y1 - y0);

    // tutaj sprawdzam czy linia nie jest horizontal lub vertical
    if (dy == 0)
    {
        draw_horizontal_line(x0, y0, (x1 - x0), color, mode);
        return;
    }
    if (dx == 0)
    {
        draw_vertical_line(x0, y0, (y1 - y0), color, mode);
        return;
    }

    //signs of x and y axes
    int8_t x_mult = (x0 > x1) ? -1 : 1;
    int8_t y_mult = (y0 > y1) ? -1 : 1;

    if (dy < dx)
    { //positive slope
        int16_t d = (2 * dy) - dx;
        uint16_t y = 0;
        for (uint16_t x = 0; x <= dx; x++)
        {
            draw_pixel(x0 + (x_mult * x), y0 + (y_mult * y), color, mode);
            if (d > 0)
            {
                y++;
                d -= dx;
            }
            d += dy;
        }
    }
    else
    { //negative slope
        int16_t d = (2 * dx) - dy;
        uint16_t x = 0;
        for (uint16_t y = 0; y <= dy; y++)
        {
            draw_pixel(x0 + (x_mult * x), y0 + (y_mult * y), color, mode);
            if (d > 0)
            {
                x++;
                d -= dy;
            }
            d += dx;
        }
    }
}

void SGL::draw_horizontal_line(uint16_t x, uint16_t y, int16_t len, uint16_t color, Mode mode)
{
    if (x >= _width)
        x = _width - 1;
    if (y >= _height)
        y = _height - 1;
    if((len + x) >= _width)
    {
        len = _width - x - 1;
    }
    if((len + x) < 0)
    {
        len = -x;
    }
    if (len > 0)
    {
        for (int16_t i = 0; i < len; ++i)
            draw_pixel(x + i, y, color, mode);
    }
    else if (len < 0)
    {
        for (int16_t i = 0; i > len; --i)
            draw_pixel(x + i, y, color, mode);
    }
}

void SGL::draw_vertical_line(uint16_t x, uint16_t y, int16_t len, uint16_t color, Mode mode)
{
    if (x >= _width)
        x = _width - 1;
    if (y >= _height)
        y = _height - 1;
    if((len + y) >= _height)
    {
        len = _height - y -1;
    }
    if((len + y) < 0)
    {
        len = -y;
    }
    if (len > 0)
    {
        for (int16_t i = 0; i < len; ++i)
            draw_pixel(x, y + i, color, mode);
    }
    else if (len < 0)
    {
        for (int16_t i = 0; i > len; --i)
            draw_pixel(x, y + i, color, mode);
    }
}

void SGL::draw_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color, Fill fill, Mode mode)
{
    if (x0 >= _width)
        x0 = _width - 1;
    if (x1 >= _width)
        x1 = _width - 1;
    if (y0 >= _height)
        y0 = _height - 1;
    if (y1 >= _height)
        y1 = _height - 1;
    int16_t dx = x1 - x0;
    int16_t dy = y1 - y0;
    if(dx == 0 && dy == 0)
    {
        draw_pixel(x0, y0, color, mode);
    }

    if (dx == 0)
    {
        draw_vertical_line(x0, y0, dy > 0 ? dy + 1 : dy - 1, color, mode);
        return;
    }
    if (dy == 0)
    {
        draw_horizontal_line(x0, y0, dx > 0 ? dx + 1 : dx - 1, color, mode);
        return;
    }

    if (fill == Fill::hole)
    {
        draw_line(x0, y0, x1, y0, color, mode);
        draw_line(x0, y1, x1, y1, color, mode);
        draw_line(x0, y0, x0, y1, color, mode);
        draw_line(x1, y0, x1, y1, color, mode);
    }
    else if (fill == Fill::solid)
    {
        #if defined(FILL_RECT_HOR)
        if (dx < 0)
        {
            SWAPUINT16(x0, x1);
            dx = abs(dx);
        }
        for (uint16_t i = 0; i <= dx; ++i)
        {
            draw_line(x0 + i, y0, x0 + i, y1, color, mode);
        }
        #elif defined(FILLL_RECT_VER)
        if (dy < 0)
        {
            SWAPUINT16(y0, y1);
            dy = abs(dy);
        }
        for (uint16_t i = 0; i <= dy; ++i)
        {
            draw_line(x0, y0 + i, x1, y0 + i, color, mode);
        }
        #endif
    }
}

void SGL::draw_triangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                        uint16_t color, Fill fill, Mode mode)
{
    if (x0 >= _width)
        x0 = _width - 1;
    if (x1 >= _width)
        x1 = _width - 1;
    if (x2 >= _width)
        x2 = _width - 1;
    if (y0 >= _height)
        y0 = _height - 1;
    if (y1 >= _height)
        y1 = _height - 1;
    if (y2 >= _height)
        y2 = _height - 1;

    if (fill == Fill::hole)
    {
        draw_line(x0, y0, x1, y1, color, mode);
        draw_line(x0, y0, x2, y2, color, mode);
        draw_line(x1, y1, x2, y2, color, mode);
    }
    else if (fill == Fill::solid)
    {
        int16_t a, b, y, last;
        // Sort coordinates by Y order (y2 >= y1 >= y0)
        if (y0 > y1)
        {
            SWAPINT16(y0, y1);
            SWAPINT16(x0, x1);
        }
        if (y1 > y2)
        {
            SWAPINT16(y2, y1);
            SWAPINT16(x2, x1);
        }
        if (y0 > y1)
        {
            SWAPINT16(y0, y1);
            SWAPINT16(x0, x1);
        }

        if (y0 == y2)
        { // Handle awkward all-on-same-line case as its own thing
            a = b = x0;
            if (x1 < a)
                a = x1;
            else if (x1 > b)
                b = x1;
            if (x2 < a)
                a = x2;
            else if (x2 > b)
                b = x2;
            draw_horizontal_line(a, y0, b - a + 1, color, mode);
            return;
        }

        int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
                dx12 = x2 - x1, dy12 = y2 - y1;
        int32_t sa = 0, sb = 0;

        // For upper part of triangle, find scanline crossings for segments
        // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
        // is included here (and second loop will be skipped, avoiding a /0
        // error there), otherwise scanline y1 is skipped here and handled
        // in the second loop...which also avoids a /0 error here if y0=y1
        // (flat-topped triangle).
        if (y1 == y2)
            last = y1; // Include y1 scanline
        else
            last = y1 - 1; // Skip it

        for (y = y0; y <= last; y++)
        {
            a = x0 + sa / dy01;
            b = x0 + sb / dy02;
            sa += dx01;
            sb += dx02;
            /* longhand:
            a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
            b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
            */
            if (a > b)
                SWAPINT16(a, b);
            draw_horizontal_line(a, y, b - a + 1, color, mode);
        }

        // For lower part of triangle, find scanline crossings for segments
        // 0-2 and 1-2.  This loop is skipped if y1=y2.
        sa = (int32_t)dx12 * (y - y1);
        sb = (int32_t)dx02 * (y - y0);
        for (; y <= y2; y++)
        {
            a = x1 + sa / dy12;
            b = x0 + sb / dy02;
            sa += dx12;
            sb += dx02;
            /* longhand:
            a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
            b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
            */
            if (a > b)
                SWAPINT16(a, b);
            draw_horizontal_line(a, y, b - a + 1, color, mode);
        }
    }
}

void SGL::draw_circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t color, Fill fill, Mode mode)
{
    //if(x0 >= _width)  x0 = _width - 1;
    //if(y0 >= _height) y0 = _height - 1;
    // from http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
    int16_t x = radius;
    int16_t y = 0;
    int16_t radiusError = 1 - x;

    while (x >= y)
    {

        // if transparent, just draw outline
        if (fill == Fill::hole)
        {
            draw_pixel(x + x0, y + y0, color, mode);
            draw_pixel(-x + x0, y + y0, color, mode);
            draw_pixel(y + x0, x + y0, color, mode);
            draw_pixel(-y + x0, x + y0, color, mode);
            draw_pixel(-y + x0, -x + y0, color, mode);
            draw_pixel(y + x0, -x + y0, color, mode);
            draw_pixel(x + x0, -y + y0, color, mode);
            draw_pixel(-x + x0, -y + y0, color, mode);
        }
        else
        { // drawing filled circle, so draw lines between points at same y value

            draw_line(x + x0, y + y0, -x + x0, y + y0, color, mode);
            draw_line(y + x0, x + y0, -y + x0, x + y0, color, mode);
            draw_line(y + x0, -x + y0, -y + x0, -x + y0, color, mode);
            draw_line(x + x0, -y + y0, -x + x0, -y + y0, color, mode);
        }

        y++;
        if (radiusError < 0)
        {
            radiusError += 2 * y + 1;
        }
        else
        {
            x--;
            radiusError += 2 * (y - x) + 1;
        }
    }
}

void SGL::draw_char(char c, uint16_t x, uint16_t y) // for the new font
// bedzie problem z pusta linia po lewej znaku - w szerokosci fontu sie tego nie uwzglednia, a nie wszystkie znaki to maja
// podsumowujac czasem jest, czasem nie ma pustej linii z lewej - nie ma informacji kiedy jest
// trzeba by sprobowac rysowac je ze stala szerokoscia fontu, nie chara
// lub cos usunac ta linie z char gdzie wystepuje i ja szcztucznie dorysowywac
// w creatorze fontow mozna je wszystkie przysnapowac do lewej, nastepnie na podstawie szerokosci narysowac i dododac space z prawej
// lub dodawac tylko w draw sring, chociaz chyba w draw char bedzie prosciej
// moze stworzyc klase, strukture, statyczna strukture, jedna tabela zawiera tylko szerokosci znakow, druga zawiera bitmapy
// od razu dodac wysokosc i ile bajtow wychodzi na kolumne
{
    if( c < 32 || c > 127)
        return;
    x_cursor = x;
    y_cursor = y;
    c = c - _font->first_char;
    uint8_t char_width = _font->get_char_width(c); // first number in row means char width
    for(uint8_t i = 1 ; i < char_width * _font->byte_mult; i += _font->byte_mult)
    {
        for(int j = 0; j < _font->byte_mult; j++)
        {
            uint8_t off = 0;
            if (j == _font->byte_mult - 1)
            {
                off = _font->byte_mult * 8 - _font->font_height;
            }
            uint8_t ch = _font->font_array[c*(_font->font_width*_font->byte_mult+1) + i + j];
            for(int8_t b = 0; b < 8 - off; ++b) //
            {
                if(((ch >> b) % 2) == !_font->invert) // == 0 or != 1 means text inverted
                    draw_pixel(x_cursor, y_cursor, _font->color);
                y_cursor++;
            }
        }
        x_cursor++;
        y_cursor = y;
    }
}

void SGL::draw_string(const char* c, uint16_t x, uint16_t y)
{
    uint8_t xx = x;
    uint8_t yy = y;
    for(; *c != '\0'; c++) {
        if(x > _width - _font->get_char_width(*c-32) && _font->wrap == false)
        {
            return;
        }
        if(x > _width - _font->get_char_width(*c-32))
        {
            x = xx;
            y += _font->font_height;
        }
        if(*c > 126)
        {
            draw_char((char)127, x, y);
        }
        else
        {
            draw_char(*c, x, y);
        }
        x += _font->get_char_width(*c-32);
    }
}

void SGL::draw_bitmap16(uint16_t* bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    // TO DO CHECK IF THIS IS CORRECT
    if(x >= _width)
        x = _width - 1;
    if(y >= _height)
        y = _height - 1;
    if((x + width) >= _width)
        width = _width - x - 1;
    if((y + _height) >= _height)
        _height = _height - y - 1;
    for(uint16_t i = 0; i <= width; i++)
    {
        for(uint16_t j = 0; j <= height; j++)
        {
            draw_pixel(i + x, j + x, *(bitmap + i + j * width));
        }
    }
}