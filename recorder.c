#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "recorder.h"



int parse(char *buffer, int count, Packet **packets, uint32_t time){

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

    (*packets)[count].time =  time;

    if (strlen(str) > 19) {
        printf("Invalid length on telemetry!\n");
        return 2;
    }

    int num_telemetry = 0;
    if(strcmp(str, "DATA") == 0){
        num_telemetry = 1;
    }
    else if (strcmp(str, "WARN") == 0) {
        num_telemetry = 2;
    }
    else if (strcmp(str, "ERR") == 0) {
        num_telemetry = 3;
    }
    else {
        printf("Invalid telemetry tag!\n");
        return 3;
    }

    (*packets)[count].telemetry = num_telemetry;

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
        char time_buffer[20];
        time_t time = data[i].time;
        struct tm *time_info = localtime(&time);
        strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", time_info);

        char telemetry[20];
        switch (data[i].telemetry) {
            case 1:
                strcpy(telemetry, "Data");
                break;
            case 2:
                strcpy(telemetry, "Warn");
                break;
            case 3:
                strcpy(telemetry, "Err");
                break;

        }

        printf("%s,%s,speed=%d,satelites=%d,battery=%d\n",
            time_buffer,
            telemetry,
            data[i].speed,
            data[i].satellites,
            data[i].battery);
    }
    return data;

}
