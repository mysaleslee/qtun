#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "network.h"
#include "main.h"

int main(int argc, char* argv[])
{
    char name[IFNAMSIZ];
    unsigned char cmd[1024];
    int localfd, remotefd;
    int rc;

    if (argc < 2)
    {
        fprintf(stderr, "usage: ./step6 <0|1>\n");
        return 1;
    }

    memset(name, 0, IFNAMSIZ);
    localfd = tun_open(name);
    if (localfd == -1) return 1;
    fprintf(stdout, "%s opened\n", name);

    switch (atoi(argv[1]))
    {
    case 1:
        if (argc < 3)
        {
            fprintf(stderr, "usage: ./step6 1 ip\n");
            return 1;
        }
        sprintf(cmd, "ifconfig %s 10.0.1.2 mtu 1492 up", name);
        rc = system(cmd);
        sprintf(cmd, "route add -net 10.0.1.0/24 dev %s", name);
        rc = system(cmd);
        sprintf(cmd, "route add 8.8.8.8 dev %s", name);
        rc = system(cmd);
        remotefd = connect_server(argv[2], 6687);
        if (remotefd == -1) return 1;
        client_loop(remotefd, localfd);
        break;
    case 2:
        if (argc < 3)
        {
            fprintf(stderr, "usage: ./step6 2 ip\n");
            return 1;
        }
        sprintf(cmd, "ifconfig %s 10.0.1.3 mtu 1000 up", name);
        rc = system(cmd);
        sprintf(cmd, "route add -net 10.0.1.0/24 dev %s", name);
        rc = system(cmd);
        sprintf(cmd, "route add 8.8.8.8 dev %s", name);
        rc = system(cmd);
        remotefd = connect_server(argv[2], 6687);
        if (remotefd == -1) return 1;
        client_loop(remotefd, localfd);
        break;
    default:
        sprintf(cmd, "ifconfig %s 10.0.1.1 mtu 1000 up", name);
        rc = system(cmd);
        sprintf(cmd, "route add -net 10.0.1.0/24 dev %s", name);
        rc = system(cmd);
        remotefd = bind_and_listen(6687);
        if (remotefd == -1) return 1;
        server_loop(remotefd, localfd);
        break;
    }
    return 0;
}
