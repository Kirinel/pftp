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
    int portnum = 21;
    char *username = "anonymous";
    char *password = "user@localhost.localnet";
    char *logfile = NULL;
    char *parafile = NULL;
    int parflag = 2;

    while ((c = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
        switch (c) {
            case 't':
                parafile = optarg;
                parflag = 0;
                break;
            case 's':
                if(parflag != 2) {
                    fprintf(stderr, "syntax error!\n");
                    exit(4);
                }
                strcpy(server->name, optarg);
                parflag = 1;
                break;
            case 'f':
                if(parflag != 1) {
                    fprintf(stderr, "syntax error!\n");
                    exit(4);
                }
                strcpy(client->filename, optarg);
                parflag = 0;
                break;
            case 'h':
                printf("Usage:\n"
                       "pftp [-s hostname] [-f file] [options] pftp -h | --help\n"
                       "pftp -v | --version\n\n"
                        "help: -h or --help\n"
                       "\tPrints a synopsis of the application usage and exits with return code 0.\n"
                       "-v or --version\n"
                       "\tPrints the name of the application, the version number (in this case the version has to\n"
                       "be 0.1), the author, and exits, returning 0. \n"
                       "[-f file] or [--file file]\n"
                       "\tSpecifies the file to download. \n"
                       "[-s hostname] or [--server hostname]\n"
                       "\tSpecifies the server to download the file from\n"
                       " \n"
                       "Options:\n"
                       "[-p port] or [--port port]\n"
                       "\tSpecifies the port to be used when contacting the server. (default value: 21).\n"
                       "[-n user] or [--username user]\n"
                       "\tUses the username user when logging into the FTP server (default value: anonymous).\n"
                       "[-P password] or [--password password]\n"
                       "\tUses the password password when logging into the FTP server (default value:user@localhost.localnet).\n"
                       "[-m mode] or [--mode mode]\n"
                       "\tSpecifies the mode to be used for the transfer (ASCII or binary) (default value: binary).\n"
                       "[-l logfile] or [--log logfile]\n"
                       "\tLogs all the FTP commands exchanged with the server and the corresponding replies to file logfile. If the filename is \"-\" then the commands are printed to the standard output.\n"
                       "\nParallel FTP Download\n"
                       "Usage: pftp [-t para-config_file] [options]\n"
                       "[-t para-config-file] or [--thread config-file]\n"
                       "\tEach line in the config-file specifies the login, password, hostname and absolute path to the file.\n"
                       "Note: pftp can use the --thread option simultaneously with the -l option. And pftp only supports binary model not ASCII mode.\n"
                       "For each line of the para-config file, the format should be: ftp://username:password@servername/file-path.\n"
                       "An example of the para-configuration file is as follows\n"
                       "\tftp://cs23300:youcandoit@ftp1.cs.uchicago.edu/rfc959.pdf\n"
                       "\tftp://socketprogramming:rocks@ftp2.cs.uchicago.edu/rfc959.pdf\n");
                *flag = 1;
                break;
            case 'v':
                printf("version: 1.0.0\n");
                *flag = 1;
                break;
            case 'p':
                if(parflag) {
                    fprintf(stderr, "syntax error!\n");
                    exit(4);
                }
                portnum = atoi(optarg);
                server->port = portnum;
//                printf("portnumber: %d\n", portnum);
                break;
            case 'n':
                if(parflag) {
                    fprintf(stderr, "syntax error!\n");
                    exit(4);
                }
                username = optarg;
//                printf("username: %s\n", client->name);
                break;
            case 'P':
                if(parflag) {
                    fprintf(stderr, "syntax error!\n");
                    exit(4);
                }
                password = optarg;
//                printf("password: %s\n", client->password);
                break;
            case 'l':
                if(parflag) {
                    fprintf(stderr, "syntax error!\n");
                    exit(4);
                }
                logfile = optarg;
//                printf("logfile: %s\n", logfile);
                break;
            default:
                fprintf(stderr, "syntax error!");
                exit(4);
        }
    }
    server->port = portnum;
    client->logfile = logfile;
    client->parafile = parafile;
    strcpy(client->name, username);
    strcpy(client->password, password);
    return 0;
}