//
// Created by 闫传麒 on 2018/11/8.
//


//!!!!!! <> indicate the library from system
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>

//!!!!!! <> indicate the library from project,or search path exactly
// project disk path is include in search path
#include "analyze.h"
#include "connect.h"

int error(char *msg)
{
    printf("%s\n", msg);
    return 0;
}

int tryconnect(user_info *user, host_info *host)
{
    int sockfd;
    struct hostent* server;
    struct sockaddr_in serv_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(host->name);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(host->port);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    //Prepare buffer should not write here
    //It is transfer logic
    // char buffer[256];
    // bzero(buffer, 256);

    //When we got here, it means sockfd is availale
    //then sockfd should return in order to use in transfer
    return sockfd;
}

int commandsend(int sockfd)
{
    //Buffer for receive
    char buffer[1024];
    //Request
    char *req = NULL;
    size_t size;
    //Loop
    while (req == NULL || strcmp(req, "EOF\n") != 0) {
        //Receive first after connect
        ssize_t recv_result = recv(sockfd, buffer, 1024, 0);
        if(recv_result >= 0) {
            printf("Res Message:\n %.*s", (int)recv_result, buffer);
        }
        //Send command
        printf("Command: \n");
        getline(&req, &size, stdin);
        ssize_t send_result = send(sockfd, req, strlen(req), 0);
        if (send_result >= 0) {
            printf("Req Message:\n%s", req);
        }
    }
    free(req);
    free(buffer);
    return 0;
}