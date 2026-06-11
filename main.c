#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


int main(){

    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock == - 1) {
        printf("Error! Can not initialize socket.\n");
        return 1;
    }

    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(3000);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int bind_sock = bind(sock, (struct sockaddr*)&server_addr,
        sizeof(server_addr));

    if (bind_sock == -1) {
        printf("Error! Can not bind socket.\n");
        return 1;
    }

    printf("[UDP] Server is listening on port 3000...\n");

    char buffer[1024];

    struct sockaddr_in client_addr;

    socklen_t client_len = sizeof(client_addr);

    int received_bytes = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&client_addr, &client_len);

    if (received_bytes == -1) {
        perror("recvfrom\n");
    }

    buffer[received_bytes] = '\0';

    printf("%s\n", buffer);

    close(sock);

    return 0;
}
