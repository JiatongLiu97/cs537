#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>




int main(int argc, char *argv[]){
         /*
         * This section is used to parse the command line
         * It will print error message if the command line entered by user is illegal.
         */
        int opt;
        int file = 0; // file = 1 means the dictionary is from a file; file = 0 means dictionary is from stdin.
        FILE *fp;

        while((opt = getopt(argc, argv, ":f:Vh")) != -1){

                switch(opt){
                        case 'V':
                                printf("my-rev from CS537 Summer 2021\n");
                                exit(0);
                        case 'h':
                                printf("my-rev is a function which reverses lines in a file\n");
                                printf("my-rev has optional options -V -h -f(followed"
                                               " by a filename)\n");
                                exit(0);
                        case 'f':
                                file = 1;
                                fp = fopen(optarg, "r");
                                if (fp == NULL){
                                        printf("my-rev: cannot open file\n");
                                        exit(1);
                                }
                                break;
                        case ':':
                                printf("my-rev: invalid command line\n");
                                exit(1);
                        case '?':
                                printf("my-rev: invalid command line\n");
                                exit(1);
               }
        }
        // If command line has extra argument, print the error message and exit(0).
	if (optind != argc){
        	printf("my-rev: invalid command line\n");
                exit(1);
	} 
        
        
        if (file == 0){
                fp = stdin;// The dictionary is from stdin.
        }
        char str[128]; // A buffer stores each line from dictionary.
        // This while loop reverses every line in the file. Print output to stdout.
        while(fgets(str, 128, fp) != NULL ){
                int length = strlen(str);
		char rev[length + 1];
		int newLine = 1;
		if ( str[length-1] != '\n' ){
			newLine = 0;
			length += 1;
		}
		for (int i = length-2; i >= 0; --i){
			rev[length - 2 - i] = str[i];
		}
		if(newLine){
			rev[length - 1] = '\n';
			rev[length] = '\0';
		}
		else{
			rev[length - 1] = '\0';
		}
		printf("%s", rev);

        }
        fclose(fp);

        exit(0);


}

