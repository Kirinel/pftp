
typedef struct user_info
{
    char *username;
    char *password;
} user_info;

typedef struct host_info
{
    char *name;
    char *port;
} host_info;

int analyzeparameter(int argc, char *argv[], user_info *user, host_info *host);
