#include "sgl.h"


#ifndef _swap_int16_t
#define _swap_int16_t(a, b)                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }
#endif

/*
plotLine(x0,y0, x1,y1)
    dx = x1 - x0
    dy = y1 - y0
    D = 2*dy - dx
    y = y0

    for x from x0 to x1
        plot(x,y)
        if D > 0
               y = y + 1
               D = D - 2*dx
        end if
        D = D + 2*dy
*/


void SGL::draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color, Mode mode) {
    uint8_t dx = abs(x1 - x0);
    uint8_t dy = abs(y1 - y0);

    //use faster algorithms for horizontal and vertical lines
    /*
    if (dy == 0) {
        draw_hline(x0, x1, y0, pattern, mode);
        return;
    }
    if (dx == 0) {
        draw_vline(y0, y1, x0, pattern, mode);
        return;
    }
    */

    //signs of x and y axes
    int8_t x_mult = (x0 > x1) ? -1 : 1;
    int8_t y_mult = (y0 > y1) ? -1 : 1;

    if (dy < dx) { //positive slope
        int8_t d = (2 * dy) - dx;
        uint8_t y = 0;
        for (uint8_t x = 0; x <= dx; x++) {
            draw_pixel(x0 + (x_mult * x), y0 + (y_mult * y), color, mode);
            if (d > 0) {
                y++;
                d -= dx;
            }
            d += dy;
        }
    } else { //negative slope
        int8_t d = (2 * dx) - dy;
        uint8_t x = 0;
        for (uint8_t y = 0; y <= dy; y++) {
            draw_pixel(x0 + (x_mult * x), y0 + (y_mult * y), color, mode);
            if (d > 0) {
                x++;
                d -= dy;
            }
            d += dx;
        }
    }
}

void SGL::draw_line_test(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color, Mode mode) {
      int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    _swap_int16_t(x0, y0);
    _swap_int16_t(x1, y1);
  }

  if (x0 > x1) {
    _swap_int16_t(x0, x1);
    _swap_int16_t(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0 <= x1; x0++) {
    if (steep) {
      draw_pixel(y0, x0, color);
    } else {
      draw_pixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}