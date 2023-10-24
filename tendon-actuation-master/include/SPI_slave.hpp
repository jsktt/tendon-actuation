#ifndef SPI_SLAVE_H
#define SPI_SLAVE_H

#include <Arduino.h>

#define MOSI    7
#define MISO    16
#define SCK     17
#define SS      6

#define BUFFER_SIZE 64

void initSPI();
void clearBuffer();
byte getByte();

#endif