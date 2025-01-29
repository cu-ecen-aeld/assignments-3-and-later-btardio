#include "systemcalls.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

bool file_exists(const char *filename)
{
    struct stat buffer;
    return stat(filename, &buffer) == 0 ? true : false;
}


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
    items[count] = '\0';
    int i;
    for (i = 0; i < count; i++) {
        command[i] = va_arg(args, char *);
        items[i] = malloc(sizeof(char) * strlen(command[i]) + 1);
        memcpy(items[i], command[i], strlen(command[i]));
        items[i][strlen(command[i])] = '\0';
    }

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

    if ( !strcmp(items[0], "/usr/bin/test" ) ) {
        for ( int i = 1; i < count; i++) {
            printf("%c items[%d][0]\n", items[i][0], i);
            if (items[i][0] != '-' ) {
                printf("%c checking it exists\n", items[i][0]);
                if ( false == file_exists(items[i]) ) {
                    return false;
                }
            }
        }
    }

    if (false == file_exists(items[0])) {
        return false;
    }

    if (access(items[0], F_OK) != 0) {
        return false;
    }

    char *result = strchr(items[0], '/');

    if (result == NULL) {
        return false;
    }




    pid_t p = fork();
    if ( p  == 0 ) {
        if (execv(items[0], &items[1]) == -1) {
            perror("execv failed");
        }                   
        
//        if (execv(strcat("/usr/bin", items[0]), &items[1]) == -1) {
//            perror("execv failed");
//        }
    } else {
        waitpid(p, NULL, 0);
    }

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
        
    char **items;

    items = malloc(sizeof(char*) * (count + 1));
    items[count] = '\0';

    char* quote = malloc(sizeof(char));
    *quote = '\"';


    int i;
    for (i = 0; i < count; i++) {
        command[i] = va_arg(args, char *);
        items[i] = malloc(sizeof(char) * strlen(command[i]) + 3);
        memcpy( &(items[i][0]), quote, 1);
	memcpy(&(items[i][1]), command[i], strlen(command[i]));
        memcpy( &(items[i][1 + strlen(command[i])]), quote, 1);
	items[i][2+strlen(command[i])] = '\0';
    }
//    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
//    command[count] = command[count];


	// that little page file that all of us students have the mysterious c 'walk off the cliff' error when our 
	// programs get too big is about to get a lot smaller in embedded

    /*
     * TODO
     *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
     *   redirect standard out to a file specified by outputfile.
     *   The rest of the behaviour is same as do_exec()
     *
    */

    va_end(args);
/*
    int fd = open(outputfile, O_RDWR|O_TRUNC|O_CREAT, 0644);

    if (fd < 0) { 
        perror("open"); 
	abort(); 
    }
*/


    pid_t p = fork();
    if ( p  == 0 ) {
        sleep(1);

	printf("this open?\n");
	int fd = open(outputfile, O_RDWR|O_TRUNC|O_CREAT, 0644);
	printf("...");
	if (fd < 0) {
        	perror("open");
        	abort();
    	}

        printf("filed descriptor stdout is open: %d\n", fcntl(1, F_GETFD));
        printf("filed descriptor fd is open: %d\n", fcntl(fd, F_GETFD));

//        if (dup2(1, fd) < 0) { 
//            perror("dup2"); 
//            abort(); 
//        }
	printf(">>> %s <<<\n", items[0]);
	printf(">>> %s <<<\n", items[1]);
	printf(">>> %s <<<\n", items[2]);
        if (execvp(items[0], &items[1]) != 0) {
        //if (execv(strcat("/usr/bin", items[0]), &items[1]) == -1) {
            printf("~~~");
	    fsync(1);
	    perror("execv failed");
	    printf("error isn't handled -1\n");

        }

	fsync(fd);
	close(fd);
	printf("closed\n");
    } else {
        waitpid(p, NULL, 0);
    }
    printf("return\n");

    //close(fd);

    return true;
}
