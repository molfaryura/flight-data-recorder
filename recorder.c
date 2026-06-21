#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "recorder.h"



int parse(char *buffer, int count, Packet **packets, char* time){

    const char delimeters[] = "$,#";

    char *packet_str = buffer;

    char *str = strtok(packet_str, delimeters);

    if (str == NULL) {
            printf("Error: Received incorrect packet!\n");
            return 2;
        }


    Packet *temp = realloc(*packets, (count + 1) * sizeof(Packet));

    if(temp != NULL){
        *packets = temp;
    }
    else {
        printf("Error: Can not reallocate memory!\n");
        return 3;
    }


    strcpy((*packets)[count].time, time);
    strcpy((*packets)[count].telemetry, str);

    int i = 0;

    while ((str = strtok(NULL, delimeters)) != NULL) {

        int value = atoi(str);

        if(i == 0){
            (*packets)[count].speed = (uint8_t)value;
        }
        else if(i == 1){
            (*packets)[count].satellites = (uint8_t)value;

        }
        else if(i == 2){
            (*packets)[count].battery = (uint8_t)value;
            if(value <= 15){
                return 1;
            }
        }
        else {
            return 2;
        }

        i++;

    }
    return 0;

}


void write_to_file(char file_name[], Packet *packets, int count){


    FILE *file = fopen(file_name, "ab");

    if (file == NULL) {
        perror("fopen");
        return;
    }

    fwrite(packets, sizeof(Packet), count, file);

    fclose(file);
}

void* read_from_file(char file_name[]){

    FILE *file = fopen(file_name, "rb");

    if (file == NULL) {
        perror("fopen");
        return file;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    size_t count = file_size / sizeof(Packet);

    Packet *data = malloc(count * sizeof(Packet));

    fread(data, sizeof(Packet), count, file);

    fclose(file);

    for (int i = 0; i < count; i++) {
        printf("%s,%s,speed=%d,satelites=%d,battery=%d\n",
            data[i].time,
            data[i].telemetry,
            data[i].speed,
            data[i].satellites,
            data[i].battery);
    }
    return data;

}
