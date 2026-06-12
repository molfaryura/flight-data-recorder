#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "recorder.h"



void parse(char *buffer){

    const char delimeters[] = "$,#";

    char *packet_str = buffer;

    char *str = strtok(packet_str, delimeters);

    if (str == NULL) {
            printf("Помилка: Отримано порожній або некоректний пакет!\n");
            return;
        }

    Packet *packet = calloc(1, sizeof(Packet));

    strcpy(packet->telemetry, str);

    int i = 0;

    while ((str = strtok(NULL, delimeters)) != NULL) {

        int value = atoi(str);

        if(i == 0){
            packet->speed = (uint8_t)value;
        }
        else if(i == 1){
            packet->battery = (uint8_t)value;
        }
        else{
            packet->satellites = (uint8_t)value;
        }

        i++;

    }

}
