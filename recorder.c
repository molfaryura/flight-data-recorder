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
            printf("Помилка: Отримано порожній або некоректний пакет!\n");
            return 1;
        }


    *packets = realloc(*packets, (count + 1) * sizeof(Packet));


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
        else{
            (*packets)[count].battery = (uint8_t)value;
            if(value <= 15){
                return 1;
            }
        }

        i++;

    }
    return 0;

}


void write_to_file(Packet *packets, int count){


    FILE *file = fopen("blackbox.bin", "wb");

    fwrite(packets, sizeof(Packet), count, file);


    fclose(file);
}

void read_from_file(int count){

    FILE *file = fopen("blackbox.bin", "rb");

    Packet packets[count];

    fread(packets, sizeof(Packet), count, file);

    fclose(file);

    for (int i = 0; i < count; i++) {
        printf("%s,%s,%d,%d,%d\n", packets[i].time, packets[i].telemetry, packets[i].speed, packets[i].satellites, packets[i].battery);
    }

}
