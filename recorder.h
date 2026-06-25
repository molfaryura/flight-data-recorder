#ifndef RECORDER_H
#define RECORDER_H

#include <stdint.h>

typedef union{
    uint64_t raw;
    struct{
        uint32_t time;
        uint8_t telemetry;
        uint8_t speed;
        uint8_t battery: 7;
        unsigned int satellites: 5;
    };

} Packet;

int parse(char *buffer, int count, Packet **packets, uint32_t time);
void write_to_file(char file_name[], Packet *packets, int count);
void* read_from_file(char file_name[]);


#endif
