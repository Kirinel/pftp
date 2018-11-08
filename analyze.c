#include "analyze.h"
#include <stdio.h>
#include <getopt.h>

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
    return 0;
}