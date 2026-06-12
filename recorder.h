#ifndef RECORDER_H
#define RECORDER_H

#include <stdint.h>

typedef struct{
    char telemetry[50];
    uint8_t speed;
    uint8_t battery;
    uint8_t satellites;
} Packet;

void parse(char *buffer);

#endif
