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

int error(char *msg)
{
    printf("%s\n", msg);
    return 0;
}

int ctrlSockConnect(user_info *user, host_info *host)
{
    int sockfd;
    char writeBuf[128] ;
    char readBuf[128];
    struct sockaddr_in serv_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    user->controlSock = sockfd;
    host->controlSock = sockfd;
    struct hostent *server = gethostbyname(host->name);
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
    bzero(readBuf, 128);
    recv(host->controlSock, readBuf, 127, 0);
//    printf("readbuf: *%s*", readBuf);

    sprintf(writeBuf, "USER %s\r\n", user->username);
    send(host->controlSock, writeBuf, strlen(writeBuf), 0);
    bzero(readBuf, 128);
    recv(host->controlSock, readBuf, 127, 0);
//    printf("readbuf: +%s+", readBuf);

    sprintf(writeBuf, "PASS %s\r\n", user->password);
    send(host->controlSock, writeBuf, strlen(writeBuf), 0);
    bzero(readBuf, 128);
    recv(host->controlSock, readBuf, 127, 0);
//    printf("readbuf: ?%s?", readBuf);

    sprintf(writeBuf, "PASV\r\n");
    send(host->controlSock, writeBuf, strlen(writeBuf), 0);
    bzero(readBuf, 128);
    recv(host->controlSock, readBuf, 127, 0);
//    printf("readbuf: -%s-", readBuf);

    char *p = strtok(readBuf, ",");
    int i = 4;
    int portnum = 0;
    while(i--) {
        p = strtok(NULL, ",");
    }
//    printf("p:%s\n",p);
    portnum += atoi(p) * 256;
    p = strtok(NULL, ",");
    p[strlen(p)-4] = 0;
//    printf("next p:%s\n", p);
    portnum += atoi(p); // p = xxx).
//    printf("portnum: %d\n", portnum);
    user->dataport = portnum;
    return sockfd;
}

int dataSockConnect(user_info *user, host_info *host) {
    int sockfd;
    struct sockaddr_in serv_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    user->dataSock = sockfd;
    host->dataSock = sockfd;
    struct hostent *server = gethostbyname(host->name);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; //try bcopy the address parsed with h1~h4
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(user->dataport);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    char writeBuf[1280];
    char readBuf[1280];

    sprintf(writeBuf, "RETR %s\r\n", user->filename);
    send(user->controlSock, writeBuf, strlen(writeBuf), 0);
    bzero(readBuf, 128);
    recv(user->controlSock, readBuf, 127, 0);
//    printf("readbuf: @%s@", readBuf);

    int fileHandle = open(user->filename, O_WRONLY | O_CREAT | O_TRUNC, S_IREAD| S_IWRITE);
    int nread;
    if(fileHandle == -1) {
        error("file handle failed\n");
    }
    while(1) {
        if((nread = recv(user->dataSock,readBuf,128,0)) < 0) {
            printf("receive error\n");
        }
        else if(nread == 0) {
            break;
        }
        if(write(fileHandle,readBuf,nread) != nread)
            printf("error writing file");
    }
    return sockfd;
}
