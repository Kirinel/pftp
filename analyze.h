
typedef struct user_info
{
    char *username;
    char *password;
    char *logfile;
    char *filename;
    int dataport;
    int mode;
    int controlSock;
    int dataSock;
} user_info;

typedef struct host_info
{
    char *name;
    char address[128];
    int port;
    int dataport;
    int controlSock;
    int dataSock;

} host_info;

int analyzeparameter(int argc, char *argv[], user_info *user, host_info *host);
