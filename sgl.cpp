#include "sgl.h"


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

    if(dx == 0) {
        draw_vertival_line(x0, y0, dy);
        return;
    }
    if(dy == 0) {
        draw_horizontal_line(x0, y0, dx);
        return;
    }

    if(dx < 0);
    if(dy < 0);

    if(fill == Fill::hole) {
        draw_line(x0, y0, x1, y0, color, mode);
        draw_line(x0, y1, x1, y1, color, mode);
        draw_line(x0, y0, x0, y1, color, mode);
        draw_line(x1, y0, x1, y1, color, mode);
    }
    else if(fill == Fill::solid) {
        ;
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
        ;
    }
}
