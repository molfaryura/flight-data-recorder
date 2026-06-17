#ifndef RECORDER_H
#define RECORDER_H

#include <stdint.h>

typedef struct{
    char time[50];
    char telemetry[50];
    uint8_t speed;
    uint8_t battery;
    uint8_t satellites;
} Packet;

int parse(char *buffer, int count, Packet **packets, char* time);
void write_to_file(Packet *packets, int count);
void read_from_file(int count);


#endif
