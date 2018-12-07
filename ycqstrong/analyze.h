
#include <ntsid.h>

typedef struct user_info {
    char name[128];
    char password[128];
    char *logfile;
    char filename[128];
    char *parafile;
    int port;
    int dataport;
    int controlSock;
    int dataSock;
} user_info;

typedef struct parg {
    user_info *client;
    user_info *server;
    int argc;
    int num;
    pthread_mutex_t *mutex;
} parg;


int analyzeparameter(int argc, char *argv[], user_info *client, user_info *server, int *flag);
