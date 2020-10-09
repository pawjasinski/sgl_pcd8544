#include "sgl.h"

#ifndef _swap_int16_t
#define _swap_int16_t(a, b)                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }
#endif


void SGL::draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color, Mode mode) {
    int16_t dx = abs(x1 - x0);
    int16_t dy = abs(y1 - y0);

    if (dy == 0) {
        draw_horizontal_line(x0, y0, (x1 - x0), color, mode);
        return;
    }
    if (dx == 0) {
        draw_vertival_line(x0, y0, (y1 - y0), color, mode);
        return;
    }

    //signs of x and y axes
    int8_t x_mult = (x0 > x1) ? -1 : 1;
    int8_t y_mult = (y0 > y1) ? -1 : 1;

    if (dy < dx) { //positive slope
        int16_t d = (2 * dy) - dx;
        uint16_t y = 0;
        for (uint16_t x = 0; x <= dx; x++) {
            draw_pixel(x0 + (x_mult * x), y0 + (y_mult * y), color, mode);
            if (d > 0) {
                y++;
                d -= dx;
            }
            d += dy;
        }
    } else { //negative slope
        int16_t d = (2 * dx) - dy;
        uint16_t x = 0;
        for (uint16_t y = 0; y <= dy; y++) {
            draw_pixel(x0 + (x_mult * x), y0 + (y_mult * y), color, mode);
            if (d > 0) {
                x++;
                d -= dy;
            }
            d += dx;
        }
    }
}

void SGL::draw_horizontal_line(uint16_t x, uint16_t y, int16_t len, uint16_t color, Mode mode) {
    int16_t i = 0;
    if(len > 0) {
        for(; i <= len; ++i)
            draw_pixel(x + i, y, color, mode);
    }
    else if(len < 0) {
        for(; i >= len; --i)
            draw_pixel(x + i, y, color, mode);
    }
}

void SGL::draw_vertival_line(uint16_t x, uint16_t y, int16_t len, uint16_t color, Mode mode) {
    int16_t i = 0;
    if(len > 0) {
        for(; i != len; ++i)
            draw_pixel(x, y + i);
    }
    else if(len < 0) {
        for(; i != len; --i)
            draw_pixel(x, y + i);
    }
}

void SGL::draw_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color, Fill fill, Mode mode) {
    int16_t dx = x1 - x0;
    int16_t dy = y1 - y0;
    uint16_t tx;
    uint16_t ty;

    if(dx == 0) {
        draw_vertival_line(x0, y0, dy);
        return;
    }
    if(dy == 0) {
        draw_horizontal_line(x0, y0, dx);
        return;
    }

    if(dx > 0 && dy < 0) {
        ty = y0;
        y0 = y1;
        y1 = ty;
    }
    
    if(dx < 0 && dy > 0) {
        tx = x0;
        x0 = x1;
        x1 = tx;
    }

    if(dx < 0 && dy < 0) {
        tx = x1
        ty = y1;
        x1 = x0;
        y1 = y0;
        x0 = tx;
        y0 = ty;
    }

    dx = x1 - x0;
    dy = y1 - y0;

    if(fill == Fill::hole) {
        draw_line(x0, y0, x1, y0, color, mode);
        draw_line(x0, y1, x1, y1, color, mode);
        draw_line(x0, y0, x0, y1, color, mode);
        draw_line(x1, y0, x1, y1, color, mode);
    }
    else if(fill == Fill::solid) {
        for(uint16_t i = y0; i <= dy; ++i ) {
            draw_horizontal_line(x0, y0 + i, dx, color, mode);
        }
    }
}

void SGL::draw_triangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                                            uint16_t color, Fill fill, Mode mode) {
    if(fill == Fill::hole) {
        draw_line(x0, y0, x1, y1, color, mode);
        draw_line(x0, y0, x2, y2, color, mode);
        draw_line(x1, y1, x2, y2, color, mode);
    }
    else if(fill == Fill::solid) {
        int16_t a, b, y, last;
        // Sort coordinates by Y order (y2 >= y1 >= y0)
        if (y0 > y1) {
            _swap_int16_t(y0, y1);
            _swap_int16_t(x0, x1);
        }
        if (y1 > y2) {
            _swap_int16_t(y2, y1);
            _swap_int16_t(x2, x1);
        }
        if (y0 > y1) {
            _swap_int16_t(y0, y1);
            _swap_int16_t(x0, x1);
        }

        if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
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

        for (y = y0; y <= last; y++) {
            a = x0 + sa / dy01;
            b = x0 + sb / dy02;
            sa += dx01;
            sb += dx02;
            /* longhand:
            a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
            b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
            */
            if (a > b)
            _swap_int16_t(a, b);
            draw_horizontal_line(a, y, b - a + 1, color, mode);
        }

        // For lower part of triangle, find scanline crossings for segments
        // 0-2 and 1-2.  This loop is skipped if y1=y2.
        sa = (int32_t)dx12 * (y - y1);
        sb = (int32_t)dx02 * (y - y0);
        for (; y <= y2; y++) {
            a = x1 + sa / dy12;
            b = x0 + sb / dy02;
            sa += dx12;
            sb += dx02;
            /* longhand:
            a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
            b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
            */
            if (a > b)
                _swap_int16_t(a, b);
            draw_horizontal_line(a, y, b - a + 1, color, mode);
        }
    }
}

void draw_circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t color, Fill fill, Mode mode) {
    if (fill == Fill::hole) {
        if (!radius) {
            draw_pixel(x0, y0, color, mode);
            return;
        }

        // draw the pixels in the cardinal directions
        draw_pixel(x0 + radius, y0, color, mode);
        draw_pixel(x0 - radius, y0, color, mode);
        draw_pixel(x0, y0 + radius, color, mode);
        draw_pixel(xo, y0 - radius, color, mode);

        uint8_t x = radius; // start at the cardinal points of the circle
        uint8_t y = 1;
        int8_t dx = 3 - (2 * radius);
        int8_t dy = 1;
        int8_t err = 1; // difference of true radius squared and expected radius squared

        if (2 + dx > 0) {
            x--;
            err += dx;
            dx += 2;
        }

        while (x > y) {
            // draw each octant
            draw_pixel(x0 + x, y0 + y, color, mode);
            draw_pixel(x0 + x, y0 - y, color, mode);
            draw_pixel(x0 - x, y0 + y, color, mode);
            draw_pixel(x0 - x, y0 - y, color, mode);
            draw_pixel(x0 + y, y0 + x, color, mode);
            draw_pixel(x0 + y, y0 - x, color, mode);
            draw_pixel(x0 - y, y0 + x, color, mode);
            draw_pixel(x0 - y, y0 - x, color, mode);

            y++;
            err += dy;
            dy += 2;

            if (2 * err + dx > 0) {
                x--;
                err += dx;
                dx += 2;
            }
        }

        //draw 45° pixels
        draw_pixel(x0 + x, y0 + y, color, mode);
        draw_pixel(x0 - x, y0 + y, color, mode);
        draw_pixel(x0 + x, y0 - y, color, mode);
        draw_pixel(x0 - x, y0 - y, color, mode);
    }
    esle if(fill == FILL::solid) {
        if (!radius) {
        draw_pixel(x0, y0, pattern, mode);
        return;
    }

    draw_vertical_line(y0 - radius, y0 + radius, x0, color, mode);

    uint16_t x = radius; // start at the cardinal points of the circle
    uint16_t y = 1;
    int16_t dx = 3 - (2 * radius);
    int16_t dy = 1;
    int16_t err = 1; // difference of true radius squared and expected radius squared

    while (x > y) {
        draw_vertical_line( y0 - x, y0 + x, x0 + y, color, mode);
        ddraw_vertical_line(y0 - x, y0 + x, x0 - y, color, mode);

        y++;
        err += dy;
        dy += 2;

        if (2 * err + dx > 0) {
            x--;
            err += dx;
            dx += 2;
            draw_vertical_line(y0 - (y - 1), y0 + (y - 1), x0 + (x + 1), color, mode);
            draw_vertical_line(y0 - (y - 1), y0 + (y - 1), x0 - (x + 1), pcolor, mode);
        }
    }

    draw_vertical_line( y0 - y, y0 + y, x0 + x, color, mode);
    ddraw_vertical_line(y0 - y, y0 + y, x0 - x, color, mode);
    }
}   