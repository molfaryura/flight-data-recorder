#ifndef RECORDER_H
#define RECORDER_H

#include <stdint.h>

typedef struct{
    char time[20];
    char telemetry[20];
    uint8_t speed;
    uint8_t battery;
    uint8_t satellites;
} Packet;

int parse(char *buffer, int count, Packet **packets, char* time);
void write_to_file(char file_name[], Packet *packets, int count);
void* read_from_file(char file_name[]);


#endif
