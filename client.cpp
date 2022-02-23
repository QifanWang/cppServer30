#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


#include "./header/util.h"

int main() {
    char buf[1024];

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(-1 == sockfd, "socket creation error");

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    // local address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    errif(-1 == connect(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr)), "connection error");

    while(true) {
        bzero(buf, sizeof(buf));
        scanf("%s", buf);
        // write
        ssize_t write_bytes = write(sockfd, buf, sizeof(buf));
        errif(-1 == write_bytes, "write error");

        // read echo
        bzero(buf, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if (read_bytes > 0) {
            printf("msg from server: %s\n", buf);
        } else if (read_bytes == 0) {
            printf("server socket disconnected.\n");
            break;
        } else {
            close(sockfd);
            errif(true, "socket read error");
        }
    }
    return 0;
}

