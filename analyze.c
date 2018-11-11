#include "analyze.h"
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

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

//return 0 is success
int analyzeparameter(int argc, char *argv[], user_info *user, host_info *host)
{
    int c;
    char *hostname;
    char *filename;
    int portnum = 21;
    char *username;
    char *password;
    char *mode;
    char *logfile;

    while ((c = getopt_long(argc, argv, short_options, long_options, NULL)) != -1)
    {
        switch (c)
        {
        case 's':
            //TODO : hostname var is not necessary
            host->name = optarg;
//            hostname = optarg;
//            strcpy(host->name, hostname);
            printf("hostname: %s\n",host->name);
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
            host->port = portnum;
            printf("portnumber: %d\n", portnum);
            break;
        case 'n':
            username = optarg;
            strcpy(user->username, username);
            printf("username: %s\n", username);
            break;
        case 'P':
            password = optarg;
            strcpy(user->password, password);
            printf("password: %s\n", password);
            break;
        case 'm':
            user->mode = strcmp(optarg, "A") ? 1 : 0;
//            mode = optarg;
            printf("mode: %s\n", mode);
            break;
        case 'l':
            logfile = optarg;
            printf("logfile: %s\n", logfile);
            strcpy(user->logfile, logfile);
            break;
        }
    }
    //TODO : use wrong port
    //wrong! port 21 is using on server, not client
//    user->portno = portnum;
    host->port = portnum;
    //Do not use if..else like this!
    //use triple operator or append {}
    //And this cause your code crash , because the mode may be NULL
//    if(!strcmp(mode, "A")) user->mode = 1; //mode = ASCII
//    else user->mode = 0;
    return 0;
}