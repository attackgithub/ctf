#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "includes/cmd.h"

void cmd_mac_address(dyad_Stream *stream, int argc, char *argv[]) 
{
    // literally just list off a random mac address?
    dyad_writef(stream, "Current Mac Address: 44:D6:57:0F:6D:C5\n");
}

void cmd_ip_address(dyad_Stream *stream, int argc, char *argv[]) 
{
    // literally just list off a random ip address?
    dyad_writef(stream, "Current IP Address: 10.0.0.92\n");
}

void cmd_logout(dyad_Stream *stream, int argc, char *argv[])
{
    if (connection_authenticated) 
    {
        connection_authenticated = 0;
        dyad_writef(stream, "Successfully logged out!\n");
    } 
    else 
    {
        dyad_writef(stream, "You need to be logged in to log out. Please log in to log out.\n");
    }
}

char *hostname = NULL;

void cmd_hostname(dyad_Stream *stream, int argc, char *argv[])
{
    if (hostname == NULL)
    {
        hostname = strdup("dvrland");
    }
    
    if (argc == 2 && strcmp(argv[1], "-h") == 0)
    {
        dyad_writef(stream,
            "Available usage:\n"
            "hostname\n"
            "    - Show current hostname.\n"
            "hostname -s <hostname>\n"
            "    - Set new hostname.\n"
        );
        
        return;
    }
    
    if (argc >= 3 && strcmp(argv[1], "-s") == 0)
    {
        int size = 17 + 1 + (argc - 2);
        int i;
        for (i = 2; i < argc; i++) {
            size += strlen(argv[i] + 1);
        }
        
        char *donthackme_argv[4] = {0};
        donthackme_argv[0] = "/bin/sh";
        donthackme_argv[1] = "-c";
        donthackme_argv[2] = malloc(size);
        donthackme_argv[3] = NULL;
        sprintf(donthackme_argv[2], "sudo /bin/echo \"%s", argv[2]);
        
        if (hostname != NULL) {
            free(hostname);
        }
        hostname = malloc(size);
        strcpy(hostname, argv[2]);
        
        for (i = 3; i < argc; i++)
        {
            sprintf(donthackme_argv[2] + strlen(donthackme_argv[2]), " %s", argv[i]);
            sprintf(hostname + strlen(hostname), " %s", argv[i]);
        }
        
        sprintf(donthackme_argv[2] + strlen(donthackme_argv[2]), "\"");
        
        int sock = dyad_getSocket(stream);
        int pid = 0;
        
        if ((pid = fork()) == 0) {
            dup2(sock, 0);
            dup2(sock, 1);
            dup2(sock, 2);
            
            execve(donthackme_argv[0], donthackme_argv, NULL);
            _exit(0);
        }
        waitpid(pid, NULL, 0);
        
        dyad_writef(stream, "New Hostname: %s\n", hostname);
        
        free(donthackme_argv[2]);
        
        return;
    }
    
    dyad_writef(stream, "Current Hostname: %s\n", hostname);
}