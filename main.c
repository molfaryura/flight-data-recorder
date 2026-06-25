#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include "udp.h"
#include "recorder.h"


int main(int argc, char *argv[]){

    if(argc < 2){
        printf("Please choose between server or analytics regimes.\n");
        return 1;
    }

    if(strcmp(argv[1], "server") == 0){
        udp_socket();
    }
    else if (strcmp(argv[1], "analytic") == 0) {
        void *data = read_from_file("blackbox.bin");
        free(data);
    }
    else {
        printf("Unknown argument!\n");
        return 1;
    }

    return 0;
};

int udp_socket(){
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock == - 1) {
        printf("Error! Can not initialize socket.\n");
        return 1;
    }

    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int bind_sock = bind(sock, (struct sockaddr*)&server_addr,
        sizeof(server_addr));

    if (bind_sock == -1) {
        printf("Error! Can not bind socket.\n");
        return 1;
    }

    printf("[UDP] Server is listening on port %d...\n", PORT);
    printf("Waiting for the packet <<$TELEMETRY,speed,battery,satellites#>>...\n");

    char buffer[1024];

    struct sockaddr_in client_addr;

    socklen_t client_len = sizeof(client_addr);

    int count = 0;

    Packet *packets = calloc(1, sizeof(Packet));

    int receiving_packets = 1;

    while (receiving_packets) {
        if(count == 5){
            break;
        }

        int received_bytes = recvfrom(sock, buffer, sizeof(buffer) - 1, 0,
                                    (struct sockaddr *)&client_addr, &client_len);

        if (received_bytes == -1) {
            perror("recvfrom\n");
            close(sock);
            free(packets);
            return 1;
        }

        buffer[received_bytes] = '\0';

        time_t raw_time = time(NULL);

        int parsed_buffer = parse(buffer, count, &packets, (uint32_t)raw_time);

        if(parsed_buffer == 1){
            printf("Critical battery. Writing logs...\n");
            receiving_packets = 0;
        }
        else if (parsed_buffer == 2) {
            printf("Corrupted packet...\n");
            continue;
        }
        else if (parsed_buffer == 3) {
            break;
        }

        count++;
    }

    write_to_file("blackbox.bin", packets, count);

    free(packets);
    packets = NULL;

    close(sock);

    return 0;
}
