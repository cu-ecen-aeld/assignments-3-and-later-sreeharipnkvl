#include "systemcalls.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/	

	int status;
	int ps;
	ps=system(cmd);
	waitpid(0,&status,0);
	if ( ps==-1)
	{
		perror("Sytem command fail");
		fprintf(stderr, "do_system");
		return false;
	}
	else if(WIFEXITED(status) && WEXITSTATUS(status) == 127 )
	{
	       	fprintf(stderr,"Error with child process %d : %s",errno,strerror(errno));
		return false;
	}
	return true;

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

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
   // command[count] = command[count];

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
     if ( *command[0] != '/' || *command[2] != '/') {
        // Command does not include an absolute path
        fprintf(stderr, "Absolute path required %s\n", command[0]);
        return false;
    }
     
    fprintf(stderr, "Printing variable %s\n", command[2]); 
    pid_t pid=fork();
    if ( pid ==-1 )
    {
	    perror("Error in fork");
	    fprintf(stderr, "fork");
	    return false;
    }
    else if (pid == 0) {
        // Child process
        execv(command[0], command);
	fprintf(stderr, "Execv failed\n");
	return false;
    }
    else
    {
    	int status;
    	wait(&status);
    	if (!WIFEXITED(status))
    	{
	    fprintf(stderr, "exit_status");
	    return false;
    	}
    }

    va_end(args);

    return true;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
   // command[count] = command[count];


/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/
     va_end(args);

    if (command[0][0] != '/') {
        // Command does not include an absolute path
        fprintf(stderr, "Absolute path required\n");
        return false;
    }

    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return false;
    } else if (pid == 0) {
        // Child process

        // Open the output file
        int fd = open(outputfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            fprintf(stderr, "Failed to open output file\n");
            return false;
        }

        // Redirect stdout to the output file
        if (dup2(fd, STDOUT_FILENO) < 0) {
            fprintf(stderr, "Failed to redirect stdout\n");
            return false;
        }

        // Close the file descriptor
        close(fd);

        // Execute the command
        execv(command[0], command);

        fprintf(stderr, "Execv failed\n");
        return false;

    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            int exitStatus = WEXITSTATUS(status);
	    return exitStatus == 0;
        } else {
            fprintf(stderr, "Child process terminated abnormally\n");
            return false;
        }
    }
    return true;
}
