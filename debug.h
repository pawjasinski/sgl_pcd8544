#ifndef __DEBUG_H__
#define __DEBUG_H__
#include "mbed.h"

static UnbufferedSerial serial_port(USBTX, USBRX, 9600);
static char serial_buffer[100];

#endif