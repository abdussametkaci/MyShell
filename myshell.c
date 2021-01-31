// including libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_CAHARACTER 1000 // max number of letters to be supported
#define MAX_LIST 100        // max number of commands to be supported
#define CMD_SIZE 6          // number of own cmd commands

// take input from user
// if valid commands are entered, erturn 0. Otherwise return -1
int input(char *str);

// control whether string is empty
// if the string is empty, return 1. Otherwise return 0
int isEmpty(char *str);

// execute program with given arguments
// If an error occurs while executing the programs, error message is printed
// and return that returned value from programs.
// Otherwise, only return that returned value from programs
int execArgs(char **args);

// get own cmd commands and control whether it contains that command in command list
// If it have not found command, return -1. Otherwise return 0
int cmdHandler(char **args, char **cmdList);

// find command, execute command with arguments and print returned results
void execCommand(char **args, char **cmdList, int index);

// clear the shell
void clear(void);

// parse string to words by using delimiter
void split(char *str, char **parsed, char *delim);

// remove character '\n' from string
// the reason I use this function is that when user enter a input, input function put '\n'
// character at the end of the string
void remove_newline_char(char *str);

// check paramters of commands
// if wrong parameters are entered in that command, return -1. Otherwise return 0
int parameterError(char *cmd, char **args);

// control arguments of tekrar program
// if wrong parameters are entered, return -1. Otherwise return 0
int controlTekrar(char **args);

// control arguments of islem program
// if wrong parameters are entered, return -1. Otherwise return 0
int controlIslem(char **args);

// check that whether a string is a number
// if the string is not a number, return 0. Otherwise return 1
int isNumber(char *str);

int main()
{
    char inputString[MAX_CAHARACTER] = {'\0'};                                       // take input from user
    char *parsedArgs[MAX_LIST] = {NULL};                                             // arguments of parsed string
    char *processes[MAX_LIST] = {NULL};                                              // contains processes splitted by "|"
    char *commandList[CMD_SIZE] = {"tekrar", "islem", "cat", "clear", "exit", NULL}; // command list

    int i; // process index
    while (1)
    {
        // if valid input does not entered, continue taking input
        if (input(inputString))
            continue;

        split(inputString, processes, "|"); // split processes by "|"
        i = 0;                              // reset index
        // i st process is not NULL
        while (processes[i] != NULL)
        {
            split(processes[i], parsedArgs, " "); // split arguments by " "
            // if command is not found and arguments are not true
            if (cmdHandler(parsedArgs, commandList))
            {
                printf("yanlis bir komut girdiniz\n");
            }
            i++; // increase process index
        }
    }
    return 0;
}

int isEmpty(char *str)
{
    // str value is not null
    while (*str != '\0')
    {
        // if str value is not empty
        if (*str != ' ')
        {
            return 0; // False: it is not empty
        }
        str++; // increse str pointer
    }
    return 1; // True: str is empty
}

int input(char *str)
{
    printf("\nmyshell>>");
    fgets(str, 1000, stdin);  // gets input, NOTE: character '\n' is append at the end of taken input string
    remove_newline_char(str); // so, we remove the character '\n'
    // if str is not empty
    if (!isEmpty(str))
    {
        return 0; // No problem. Input is valid
    }

    return -1; // We have problem. Input is not valid. It is an error
}

int execArgs(char **args)
{
    int pid = fork(); // call fork
    int i;
    int result = 0; // exit status value

    // if it is a child process
    if (pid == 0)
    {
        if (strcmp("cat", args[0]) == 0)
        {
            i = execv("/bin/cat", args);
        }
        else
        {
            i = execv(args[0], args);
        }

        // if there is an error
        if (i < 0)
        {
            printf("yanlis bir komut girdiniz\n");
            exit(0); // terminate child process
        }
    }
    else
    {
        wait(&i); // waiting for child to terminate
        result = WEXITSTATUS(i);
    }

    return result;
}

int cmdHandler(char **args, char **cmdList)
{
    int i = 0;       // command list index
    int isFound = 0; // command is found

    while (cmdList[i] != NULL)
    {
        // args[0] is command name (program name)
        // if command is found
        if (strcmp(args[0], cmdList[i]) == 0)
        {
            isFound = 1; // founded
            break;
        }
        i++;
    }

    if (isFound)
    {
        // if wrong parameter is not netered for this command (args[0])
        if (!parameterError(args[0], args))
        {
            execCommand(args, cmdList, i); // execute the program
        }
        else
        {
            return -1; // We have problem. It does not work succesfully
        }
    }

    // if command is found, return 0. It means no problem.
    // otherwise, return -1. It means that we have problem. It is an error
    return isFound ? 0 : -1;
}

void execCommand(char **args, char **cmdList, int index)
{
    if (strcmp("islem", cmdList[index]) == 0) // if command is islem
    {
        int result = execArgs(args);       // excute program and result assigned returned value
        if (strcmp("cikar", args[1]) == 0) // if islem call cikar program
        {
            printf("\n%s - %s = %d", args[2], args[3], result);
        }
        else if (strcmp("topla", args[1]) == 0) // if islem call topla program
        {
            printf("\n%s + %s = %d", args[2], args[3], result);
        }
    }
    else if (strcmp("exit", cmdList[index]) == 0) // if command exit
    {
        exit(0); // terminate myshell program
    }
    else if (strcmp("clear", cmdList[index]) == 0) // if command is clear
    {
        clear(); // clear shell
    }
    else    // Otherwise
    {
        execArgs(args); // execute command with arguments
    }
}

void split(char *str, char **parsed, char *delim)
{
    int i = 0; // parsed isndex
    // if sepereted string is not NULL, parsed[i] assigned
    while ((parsed[i] = strsep(&str, delim)) != NULL)
    {
        if (isEmpty(parsed[i])) // if parsed string is empty
        {
            continue;
        }
        i++;
    }
}

void remove_newline_char(char *str)
{
    while (*str != '\0') // str is not null
    {
        if (*str == '\n') // str is newline character
        {
            *str = '\0'; // str assigned null character
            break;
        }
        str++; // increse str pointer
    }
}

void clear()
{
    printf("\033[H\033[J"); // clear console
    // this command was taken from https://www.geeksforgeeks.org/making-linux-shell-c/
}

int parameterError(char *cmd, char **args)
{
    if (strcmp("tekrar", cmd) == 0) // if command is tekrar
    {
        if (controlTekrar(args)) // control arguments of tekrar and if return error
        {
            return -1; // we have an error
        }
    }
    else if (strcmp("islem", cmd) == 0) // if command is islem
    {
        if (controlIslem(args)) // control arguments of islem and if return error
        {
            return -1; // we have an error
        }
    }
    return 0; // no error
}

int controlTekrar(char **args)
{
    int i = 0;              // argument index
    while (args[i] != NULL) // argument is not NULL
    {
        i++; // increse index
    }

    // if tekrar has less than 3 argument (tekrar is included)
    if (i < 3) // min args
    {
        return -1; // missing argument (error)
    }

    if (!isNumber(args[i - 1]) || atoi(args[i - 1]) < 0) // the last argument is not number or negative
    {

        return -1; // wrong argument
    }

    return 0; // no problem
}

int controlIslem(char **args)
{
    int i = 0;
    while (args[i] != NULL)
    {
        i++;
    }

    // if islem has not 4 argument
    if (i != 4) // min args
    {
        return -1; // missing argument
    }

    // if not islem call topla program or cikar program
    if (!(strcmp("topla", args[1]) == 0 || strcmp("cikar", args[1]) == 0))
    {
        return -1; // error
    }

    // if the last two arguments is not number
    if (!isNumber(args[2]) && !isNumber(args[3]))
    {
        return -1; // wrong argument
    }

    return 0; // no error
}

int isNumber(char *str)
{
    int i = 0;
    while (*str != '\0')
    {
        if (i == 0)
        {
            if (str[0] == '-')
            {
                str++;
            }
            i++;
        }
        if (*str < '0' || *str > '9')
        {
            return 0; // false
        }
        str++;
    }

    return 1; // true
}
