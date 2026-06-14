#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "recorder.h"



void parse(char *buffer, int count, Packet *packets){

    const char delimeters[] = "$,#";

    char *packet_str = buffer;

    char *str = strtok(packet_str, delimeters);

    if (str == NULL) {
            printf("Помилка: Отримано порожній або некоректний пакет!\n");
            return;
        }

    if(count > 0 ){
        packets = realloc(packets, count * sizeof(Packet));
    }

    strcpy(packets[count].telemetry, str);

    int i = 0;

    while ((str = strtok(NULL, delimeters)) != NULL) {

        int value = atoi(str);

        if(i == 0){
            packets[count].speed = (uint8_t)value;
        }
        else if(i == 1){
            packets[count].battery = (uint8_t)value;
        }
        else{
            packets[count].satellites = (uint8_t)value;
        }

        i++;

    }

}
