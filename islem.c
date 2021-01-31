#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int i;
    int f = fork();
    int result; // exit status value

    if (f == 0) // if child process
    {
        // argv[0] is program name
        i = execl(argv[1], argv[2], argv[3], (char*)0); // execute program with argments
        //perror("exec2: execve failed\n");
    }
    else
    {
        wait(&i); // // waiting for child to terminate
        result = WEXITSTATUS(i); // assgine exit status
    }

    return result; // resturn exit status value
}
