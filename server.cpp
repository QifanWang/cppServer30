#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

    // bind socket
    errif(-1 == bind(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr)), "bind error"); 

    // listening
    errif(-1 == listen(sockfd, SOMAXCONN), "listen error");

    // accept a connection
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr, sizeof(clnt_addr));
    int clnt_sockfd = accept(sockfd, (sockaddr *)&clnt_addr, &clnt_addr_len);
    errif(-1 == clnt_sockfd, "accept error");
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

    while(true) {
        bzero(buf, sizeof(buf));
        ssize_t read_bytes = read(clnt_sockfd, buf, sizeof(buf));
        if (read_bytes > 0) {
            printf("msg from client fd %d: %s\n", clnt_sockfd, buf);
            write(clnt_sockfd, buf, sizeof(buf));
        } else if (read_bytes == 0) {
            printf("client fd %d disconnected\n", clnt_sockfd);
            close(clnt_sockfd);
            close(sockfd);
            break;
        } else {
            close(clnt_sockfd);
            close(sockfd);
            errif(true, "read client socket");
        }
    }

    return 0;
}