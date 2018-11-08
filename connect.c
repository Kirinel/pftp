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

int tryconnect(user_info user, host_info host)
{
    // portno = atoi(argv[2]);
    // Check First, declare all values is not necessary
    int portno = atoi(host.port);
    if (portno <= 0 || portno > UINT16_MAX)
    {
        error("invalid or missing options\nusage: snc [-l] [-u] [hostname] port\n");
    }
    // int sockfd, n;
    //Create Socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("Internal error");
    }
    //Create Server
    struct hostent *server = gethostbyname(host.name);
    if (server == NULL)
    {
        error("Internal error");
    }
    //Prepare addr
    // struct sockaddr_in serv_addr, client_addr;
    // client_addr not necessary here
    struct sockaddr_in serv_addr;
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);

    //Then we got sockfd & server
    //Try Connect
    int connect_result = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (connect_result < 0)
    {
        error("Internal error");
    }

    //Prepare buffer should not write here
    //It is transfer logic
    // char buffer[256];
    // bzero(buffer, 256);

    //When we got here, it means sockfd is availale
    //then sockfd should return in order to use in transfer
    return sockfd;
}