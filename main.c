#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <transfer.h>
#include <connect.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>

int error(char* msg) {
    printf("%s\n", msg);
    exit(0);
}

char* const short_options = "s:f:hvp:n:P:m:l:";
struct option long_options[] = {
        {"server", 1, NULL, 's'},
        {"file", 1, NULL, 'f' },
        {"help", 0, NULL, 'h'},
        {"version", 0, NULL, 'v'},
        {"port", 1, NULL, 'p'},
        {"username", 1, NULL, 'n'},
        {"password", 1, NULL, 'P'},
        {"mode", 1, NULL, 'm'},
        {"log", 1, NULL, 'l'},
        {NULL, 0, NULL, 0 }
};

int main(int argc, char *argv[]) {
    int c;
    char* hostname;
    char* filename;
    int portnum = 21;
    char* username;
    char* password;
    char* mode;
    char* logfile;

    while ((c = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
        switch (c) {
            case 's':
                hostname = optarg;
                printf("hostname: %s\n", hostname);
                break;
            case 'f':
                filename = optarg;
                printf("filename: %s\n", filename);
                break;
            case 'h':
                printf("help: blablabla\n");
                break;
            case 'v':
                printf("version: 1.0.0\n");
                break;
            case 'p':
                portnum = atoi(optarg);
                printf("portnumber: %d\n", portnum);
                break;
            case 'n':
                username = optarg;
                printf("username: %s\n", username);
                break;
            case 'P':
                password = optarg;
                printf("password: %s\n", password);
                break;
            case 'm':
                mode = optarg;
                printf("mode: %s\n", mode);
                break;
            case 'l':
                logfile = optarg;
                printf("logfile: %s\n", logfile);
                break;
        }
    }

    //connection:
    int sockfd, portno, n;

    struct sockaddr_in serv_addr,client_addr;
    struct hostent *server;
    char buffer[256];

    portno = atoi(argv[2]);
    if(portno <= 0 || portno > 65535) {
        error("invalid or missing options\nusage: snc [-l] [-u] [hostname] port\n");
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Internal error");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        error("Internal error");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("Internal error");
    bzero(buffer,256);
    return 0;
}