#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include "analyze.h"
#include "connect.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#define BUFLEN 1024

int error(char *msg)
{
    fprintf(stderr, "%s\n", msg);
    return 0;
}

void *ftpconnect(void* in)
{
    struct parg *arg = (struct parg*)in;

    user_info *client = arg->client;
    user_info *server = arg->server;
    FILE *logfile = NULL;
    if(client->logfile) {
        if(strcmp(client->logfile, "-") == 0) {
            logfile = stdout;
        }
        else if(client->logfile) {
            logfile = fopen(client->logfile, "a+");
        }
    }
    int sockfd;
    char writeBuf[BUFLEN] ;
    char readBuf[BUFLEN];
    struct sockaddr_in serv_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    client->controlSock = sockfd;
    server->controlSock = sockfd;
    struct hostent *server_addr = gethostbyname(server->name);
    if (server_addr == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server_addr->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server_addr->h_length);
    serv_addr.sin_port = htons(server->port);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    bzero(readBuf, BUFLEN);
    recv(client->controlSock, readBuf, BUFLEN, 0);
    if(client->logfile) fprintf(logfile, "%d: S->C: %s", arg->num, readBuf);

    bzero(writeBuf, BUFLEN);
    sprintf(writeBuf, "USER %s\r\n", client->name);
    if(client->logfile) fprintf(logfile, "%d: C->S: %s", arg->num, writeBuf);
    send(server->controlSock, writeBuf, strlen(writeBuf), 0);
    bzero(readBuf, BUFLEN);
    recv(server->controlSock, readBuf, BUFLEN, 0);
    if(client->logfile) fprintf(logfile, "%d: S->C: %s", arg->num, readBuf);

    bzero(writeBuf, BUFLEN);
    sprintf(writeBuf, "PASS %s\r\n", client->password);
    if(client->logfile) fprintf(logfile, "%d: C->S: %s", arg->num, writeBuf);
    send(server->controlSock, writeBuf, strlen(writeBuf), 0);
    bzero(readBuf, BUFLEN);
    recv(server->controlSock, readBuf, BUFLEN, 0);
    if(client->logfile) fprintf(logfile, "%d: S->C: %s", arg->num, readBuf);

    bzero(writeBuf, BUFLEN);
    sprintf(writeBuf, "PASV\r\n");
    if(client->logfile) fprintf(logfile, "%d: C->S: %s", arg->num, writeBuf);
    send(server->controlSock, writeBuf, strlen(writeBuf), 0);
    bzero(readBuf, BUFLEN);
    recv(server->controlSock, readBuf, BUFLEN, 0);
    if(client->logfile) fprintf(logfile, "%d: S->C: %s", arg->num, readBuf);


    char *p = strtok(readBuf, ",");
    int i = 4;
    int portnum = 0;
    while(i--) {
        p = strtok(NULL, ",");
    }
    portnum += atoi(p) * 256;
    p = strtok(NULL, ",");
    p[strlen(p)-4] = 0;
    portnum += atoi(p);
    client->dataport = portnum;
//    return sockfd;
//}

//int dataSockConnect(user_info *client, user_info *server) {
//    FILE *logfile = NULL;
//    if(strcmp(client->logfile, "-") == 0) {
//        logfile = stdout;
//    }
//    else if(client->logfile) {
//        logfile = fopen(client->logfile, "a+");
//    }
    int data_sockfd;
    struct sockaddr_in data_serv_addr;
    data_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (data_sockfd < 0)
        error("ERROR opening socket");
    client->dataSock = data_sockfd;
    struct hostent *data_server_addr = gethostbyname(server->name);
    if (data_server_addr == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &data_serv_addr, sizeof(data_serv_addr));
    data_serv_addr.sin_family = AF_INET;
    data_serv_addr.sin_addr.s_addr = INADDR_ANY; //try bcopy the address parsed with h1~h4
    bcopy((char *)data_server_addr->h_addr,
          (char *)&data_serv_addr.sin_addr.s_addr,
          data_server_addr->h_length);
    data_serv_addr.sin_port = htons(client->dataport);
    if (connect(data_sockfd,(struct sockaddr *)&data_serv_addr,sizeof(data_serv_addr)) < 0)
        error("ERROR connecting");

    bzero(writeBuf,BUFLEN);
    sprintf(writeBuf, "TYPE I\r\n");
    send(client->controlSock, writeBuf, strlen(writeBuf), 0);
    if(client->logfile) fprintf(logfile, "%d: C->S: %s", arg->num, writeBuf);
    bzero(readBuf, BUFLEN);
    recv(client->controlSock, readBuf, BUFLEN, 0);
    if(client->logfile) fprintf(logfile, "%d: S->C: %s", arg->num, readBuf);

    bzero(writeBuf, BUFLEN);
    sprintf(writeBuf, "SIZE %s\r\n", client->filename);
    send(client->controlSock, writeBuf, strlen(writeBuf), 0);
    if(client->logfile) fprintf(logfile, "%d: C->S: %s", arg->num, writeBuf);
    bzero(readBuf, BUFLEN);
    recv(client->controlSock, readBuf, BUFLEN, 0);
    if(client->logfile) fprintf(logfile, "%d: S->C: %s", arg->num, readBuf);

    p = strchr(readBuf, ' ') + 1;
    int size = atoi(p);
    printf("size: %d\n", size);

    int subsize = size/arg->argc;
    int offset = subsize * arg->num;

    bzero(writeBuf, BUFLEN);
    sprintf(writeBuf, "REST %d\r\n", offset);
    send(client->controlSock, writeBuf, strlen(writeBuf), 0);
    if(client->logfile) fprintf(logfile, "%d: C->S: %s", arg->num, writeBuf);
    bzero(readBuf, BUFLEN);
    recv(client->controlSock, readBuf, BUFLEN, 0);
    if(client->logfile) fprintf(logfile, "%d: S->C: %s", arg->num, readBuf);


    bzero(writeBuf, BUFLEN);
    sprintf(writeBuf, "RETR %s\r\n", client->filename);
    send(client->controlSock, writeBuf, strlen(writeBuf), 0);
    if(client->logfile) fprintf(logfile, "%d: C->S: %s", arg->num, writeBuf);
    bzero(readBuf, BUFLEN);
    recv(client->controlSock, readBuf, BUFLEN, 0);
    if(client->logfile) fprintf(logfile, "%d: S->C: %s", arg->num, readBuf);

    char tempfile[128];
    sprintf(tempfile, "temp_%d_%s",arg->num, client->filename);
    int fileHandle = open(tempfile, O_WRONLY | O_CREAT | O_TRUNC, S_IREAD| S_IWRITE);
    int nread;
    if(fileHandle == -1) {
        error("file handle failed\n");
    }
    int left = subsize;
    int sum = 0;
    while(1) {
        if(left > BUFLEN) {
            if((nread = recv(client->dataSock,readBuf,BUFLEN,0)) < 0) {
                printf("receive error\n");
            }
            else if(nread == 0) {
                break;
            }

        }
        else {
//            printf("%d: left < buflen\n", arg->num);
            if((nread = recv(client->dataSock,readBuf,left,0)) < 0) {
                printf("receive error\n");
            }
            else if(nread == 0) {
                break;
            }
        }
        sum+=nread;
        if(write(fileHandle,readBuf,nread) != nread)
            printf("error writing file");
        left -= nread;

    }
    printf("%d: total %d received\n", arg->num, sum);
    bzero(readBuf, BUFLEN);
    recv(client->controlSock, readBuf, BUFLEN, 0);
    if(client->logfile) fprintf(logfile, "%d: S->C: %s", arg->num, readBuf);
//    return 0;
//}
//
//int closeConnect(user_info *client, user_info *server) {
//    char writeBuf[BUFLEN];
//    char readBuf[BUFLEN];
//    FILE *logfile = NULL;
//    if(strcmp(client->logfile, "-") == 0) {
//        logfile = stdout;
//    }
//    else if(client->logfile) {
//        logfile = fopen(client->logfile, "a+");
//    }

    close(client->dataSock);
    sprintf(writeBuf, "QUIT\r\n");
    if(client->logfile) fprintf(logfile, "%d: C->S: %s", arg->num, writeBuf);
    send(server->controlSock, writeBuf, strlen(writeBuf), 0);
    bzero(readBuf, BUFLEN);
    recv(server->controlSock, readBuf, BUFLEN, 0);
    if(client->logfile) fprintf(logfile, "%d: S->C: %s", arg->num, readBuf);
    close(server->controlSock);
    return 0;
}
