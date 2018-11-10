
typedef struct user_info
{
    char *username;
    char *password;
    char *logfile;
    int portno;
    int mode;
} user_info;

typedef struct host_info
{
    char *name;
    int port;
} host_info;

int analyzeparameter(int argc, char *argv[], user_info *user, host_info *host);
