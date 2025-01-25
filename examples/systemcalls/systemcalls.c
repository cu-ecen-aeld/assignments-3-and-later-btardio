#include "systemcalls.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <unistd.h>
#include <sys/wait.h>
/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd) {
    int ret = system(cmd);
    /*
     * TODO  add your code here
     *  Call the system() function with the command set in the cmd
     *   and return a boolean true if the system() call completed with success
     *   or false() if it returned a failure
    */
    if (ret == 0) return true;
    return false;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...) {
    va_list args;
    va_start(args, count);
    char *command[count + 1];

    char **items;

    items = malloc(sizeof(char*) * (count + 1));
    items[count+2] = '\0';
    int i;
    for (i = 0; i < count; i++) {
        command[i] = va_arg(args, char *);
        items[i] = malloc(sizeof(char) * strlen(command[i]) + 1);
        memcpy(items[i], command[i], strlen(command[i]) + 1);
        items[i][strlen(command[i]) + 1] = '\0';
        //items[i] = malloc(sizeof(char) * sizeof(command[i]));
        //memcpy(items[i], command[i], sizeof(command[i]));
        printf("command[i]: %s\n", items[i]);
    }
    //command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    //command[count] = command[count];

    /*
     * TODO:
     *   Execute a system command by calling fork, execv(),
     *   and wait instead of system (see LSP page 161).
     *   Use the command[0] as the full path to the command to execute
     *   (first argument to execv), and use the remaining arguments
     *   as second argument to the execv() command.
     *
    */

    va_end(args);
    //
    // printf("%d\n", getpid());
    // printf("parent process\n");
    // printf("cmd:\n");
    // printf("%s\n", items[0]);
    // printf("args:\n");
    // for (int i = 1; i < count; i++) {
    //     printf("%s\n", items[i]);
    // }

    //printf("%s\n", command[i]);

    pid_t p = fork();
    if ( p  == 0 ) {
        printf("%d\n", p);
        printf("child process created\n");
        printf("cmd:\n");
        printf("%s\n", items[0]);
        printf("args:\n");
        for (int i = 1; i < count; i++) {
            printf("%s\n", items[i]);
        }
        if (execv(strcat("/usr/bin", items[0]), &items[1]) == -1) {
            perror("execv failed");

        }
        //break;
        printf("~child is done\n");
    } else {
        waitpid(p, NULL, 0);
        printf("~parent is done\n");
    }

    printf("both get here");

    return true;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...) {
    va_list args;
    va_start(args, count);
    char *command[count + 1];
    int i;
    for (i = 0; i < count; i++) {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];


    /*
     * TODO
     *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
     *   redirect standard out to a file specified by outputfile.
     *   The rest of the behaviour is same as do_exec()
     *
    */

    va_end(args);

    return true;
}
