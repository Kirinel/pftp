#include "analyze.h"
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

char* const short_options = "t:s:f:hvp:n:P:l:";
struct option long_options[] = {
        {"thread", 1, NULL, 't'},
        {"server", 1, NULL, 's'},
        {"file", 1, NULL, 'f' },
        {"help", 0, NULL, 'h'},
        {"version", 0, NULL, 'v'},
        {"port", 1, NULL, 'p'},
        {"username", 1, NULL, 'n'},
        {"password", 1, NULL, 'P'},
        {"log", 1, NULL, 'l'},
        {NULL, 0, NULL, 0 }
};

//return 0 is success
int analyzeparameter(int argc, char *argv[], user_info *client, user_info *server, int *flag)
{
    int c;
    char *filename;
    int portnum = 21;
    char *logfile = NULL;
    char *parafile = NULL;

    while ((c = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
        switch (c) {
            case 't':
                parafile = optarg;
                printf("parafile: %s\n",parafile);
                break;
            case 's':
                strcpy(server->name, optarg);
                printf("hostname: %s\n",server->name);
                break;
            case 'f':
                filename = optarg;
                strcpy(client->filename, optarg);
                printf("filename: %s\n", filename);
                break;
            case 'h':
                printf("help: blablabla\n");
                *flag = 1;
                break;
            case 'v':
                printf("version: 1.0.0\n");
                *flag = 1;
                break;
            case 'p':
                portnum = atoi(optarg);
                server->port = portnum;
                printf("portnumber: %d\n", portnum);
                break;
            case 'n':
                strcpy(client->name, optarg);
                printf("username: %s\n", client->name);
                break;
            case 'P':
                strcpy(client->password, optarg);
                printf("password: %s\n", client->password);
                break;
            case 'l':
                logfile = optarg;
                printf("logfile: %s\n", logfile);
                break;
        }
    }
    server->port = portnum;
    client->logfile = logfile;
    client->parafile = parafile;
    return 0;
}