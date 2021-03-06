#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdbool.h>


// This function implements built-in exit command
void exitCmd() {
        exit(0);
}

// This function implements built-in cd command
void cdCmd(char* dirName) {
    chdir(dirName);  // dirNmae is the name of directory the process cd to
    return;
}

/*
 * Usage: This function executes the  command 
 * parameter:  comArgs: array storing arguments of the command.
 * Return : viod
 */
void execCommand(char** args, char* outFile){
         
	//  Execute exit built-in cd command 
	if (strcmp(args[0], "exit") == 0 && args[1] == NULL){
            exitCmd();
	}
	//  Execute cd built-in cd command
	else if (strcmp(args[0], "cd") == 0 && args[1] != NULL && args[2] == NULL){
	    cdCmd(args[1]);
	    return;
	}
	//  Fork a child process
	pid_t pid = fork();
	//  Execute child process
	if (pid == 0){
	    if (outFile != NULL){  // If there is a redirection in the command, replace stdout with the output file stream
		int fd;
                if ((fd = open(outFile, O_CREAT|O_TRUNC|O_WRONLY, 0644)) < 0) {
		    char errMes[strlen("Cannot write to file ") + strlen(outFile) + strlen(".\n") + 1];
		    strcpy(errMes, "Cannot write to file ");
		    strcat(errMes, outFile);
		    strcat(errMes, ".\n");
                    write(2, errMes, strlen(errMes));
                    return;
                }
                dup2(fd, 1);
		close(fd);
            }
	    int i = execv(args[0], args);  // Invoke corresponding system command file
	    if (i == -1){  // Error of invoking the system command file
	        char cmd[strlen(args[0]) + strlen(": Command not found.\n") + 1]; 
	       	strcpy(cmd, args[0]);
		strcat(cmd, ": Command not found.\n");
                write(2, cmd, strlen(cmd));
		_exit(1);
	    }
	    _exit(0);
            
	}
	else{  // Wait for the child to terminate
	    waitpid(pid, NULL, 0);
	    return;
	}
}

/*
 * Usage: parse the string, extract tokens in to an array ignoring spaces
 * parameter:  str: the string to extract from
 *             tokens: an array storing all tokens
 * Return: i: the index of last element in tokens
 *        -1: the string contains only spaces
 */

int parseSpace(char* str, char** tokens){
    const char c[2] = " ";
    char *token;
    char firChr = '\0';
    char* tructedStr;
    
    for (int i = 0; i < strlen(str); i++){
        if (isspace(str[i]) == 0){
	    firChr = str[i];
	    break;
	}
    }
    if (firChr == '\0'){
        return -1;
    }
    tructedStr = strchr(str, firChr);
    // Read each token from a string into an array of tokens.
    token = strtok(tructedStr, c);
    int i = 0;
    tokens[i] = strtok_r(token, "\t", &token);
    while(token != NULL){
        i++;
        token = strtok(NULL, c);
	if (token == NULL){
	    i--;
        }
	else{
	    tokens[i] = strtok_r(token, "\t", &token);
	}
    }
    return i;
}


/*
 * Usage: parse the command line into an array of arguments
 * parameter:  inputCmd: a pointer to the command string
 *             cmdArgs: array storing arguments of the command.
 * Return: 1: means the command is invalid
 *         0: the command does not includes redirection
 *         >1: the index of the filename in cmdArgs
 */
int parseCommand(char* inputCmd, char** cmdArgs){
	char* fileName;

	if (strlen(inputCmd) == 0){
	    return 1;
	}

	//  Count the number of '>' in the command string
	int count = 0;
	for(int i = 0; i < strlen(inputCmd); i++){
	    if(inputCmd[i] == '>'){
	        count++;
	    }
	}
	if(count > 1){  // The redirection form is wrong: two '>' in the command
	    write(2, "Redirection misformatted.\n", strlen("Redirection misformatted.\n"));
	    return 1;
	}
	else if (count == 0){  // The command does not include a redirection
	    int index;
	    index = parseSpace(inputCmd, cmdArgs);
	    if (index == -1){
                return 1;
	    }
	    cmdArgs[index + 1] = NULL;
            return 0;
	}
	else{  // This command includes a '>'
	    char* cmd;
	    const char r[2] = ">";
	    cmd = strtok(inputCmd, r);
	    fileName = strtok(NULL, r);
	    char* file1;
	    char* file2;
	    const char s[2] = " ";
	    file1 = strtok(fileName, s);
	    file2 = strtok(NULL, s);
	    if(fileName == NULL){  // The redirection form is wrong: Missing sides of '>'
	        write(2, "Redirection misformatted.\n", strlen("Redirection misformatted.\n"));
	        return 1;
	    }
	    else if(file2 != NULL){  // The redirection form is wrong: Multiple output files 
                write(2, "Redirection misformatted.\n", strlen("Redirection misformatted.\n")); 
                return 1;
	    }
	    else{
		int fileIndex;
		int index;

		index = parseSpace(cmd, cmdArgs);
		if (index == -1){
		    return 1;
		}
		fileIndex = index + 1;
		cmdArgs[fileIndex] = file1;
		return fileIndex;
	    }
        }
  
}	


int main( int argc, char *argv[] )  {
    FILE *fp; //  File pointer of batch file.
    char command[512] = {0};
    char* cmdArgs[100];  // An array stores arguments of a command.
    char* file = NULL;


    //  Incorrect command line arguments, print error message.
    if (argc > 2){
        write(2, "Usage: mysh [batch-file]\n", strlen("Usage: mysh [batch-file]\n"));
        exit(1);
    }
    //  Batch Mode
    else if (argc == 2){
	fp = fopen(argv[1], "r");
	//  File can not be opened. Exit with code 1.
	if (fp == NULL){
	    char str[strlen("Error: Cannot open file ") + strlen(argv[1]) + strlen(".\n") + 1];
	    strcpy(str, "Error: Cannot open file ");
	    strcat(str, argv[1]);
	    strcat(str, ".\n");
            write(2, str, strlen(str));
	    exit(1);
	}
	//  Start batch mode.
	else{
	    while (fgets(command, 512, fp)){
	        write(1, command, strlen(command));
		command[strcspn(command, "\n")] = 0;  // Eliminate the trailing new line char
                int f = parseCommand(command, cmdArgs);  // Parse the commandi
		if (f == 1){  // The command is invalid. Skip this command.
		    continue;
	        }
                else if(f == 0){  // The command does not include redirection
                    execCommand(cmdArgs, file);  // Execute the command
		}
		else{  // The command includes a redirection
                    file = cmdArgs[f];
		    cmdArgs[f] = NULL;
                    execCommand(cmdArgs, file);  // Execute the command
		}
		file = NULL;
            }
	    //  Close the file.
	    if (fclose(fp)){
	        return EXIT_FAILURE;
	        perror(argv[1]);
	    }
	}
    }
    // Interactive Mode
    else{
	write(1, "mysh> ", strlen("mysh> "));//  Present the prompt to user.
        while (fgets(command, 512, stdin)){
	     
	     command[strcspn(command, "\n")] = 0;  // Eliminate the trailing new line char
	     // Parsing the command from stdin here.
	     int f;
	     f = parseCommand(command, cmdArgs);
	     if (f == 1){  // The command is invalid. Discard this command.

             }
	     else if(f == 0){  // The command does not include redirection
                 execCommand(cmdArgs, file);  // Execute the command
	     }
	     else{  // The command includes a redirection
		 file = cmdArgs[f];
                 cmdArgs[f] = NULL;
                 execCommand(cmdArgs, file);  // Execute the command
             }
	     file = NULL;
	     write(1, "mysh> ", strlen("mysh> "));//  Present the prompt to user.
	}
    }

    exit(0);
}
