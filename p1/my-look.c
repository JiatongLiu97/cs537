//  Copyright [2021] <Jiatong Liu>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]) {
	 /*
         // Make sure there is a specified string at the end of the command.
	 if (argc <= 1 || argv[argc-1][0] =='-' || strcmp(argv[argc-2],"-f") == 0){
		 printf("my-look: invalid command line\n");
                 exit(1);
         }
	 */


	 /*
	 * This section is used to parse the command line
	 * It will print error message if the command line entered by user is illegal.
	 */
	int opt;
	//  file = 1 means the dictionary is from a file
	//  file = 0 means dictionary is from stdin
	int file = 0;
       	FILE *fp;

	while((opt = getopt(argc, argv, ":f:Vh")) != -1) {
		switch(opt) {
			case 'V':
				printf("my-look from CS537 Summer 2021\n");
				exit(0);
			case 'h':
				printf("my-look is a function searching lines in a file"
					      "which contain specified prefix.\n");
				printf("my-look has optional options -V -h -f(followed"
				               " by a filename) and a required search string at the end\n");
			        exit(0);
			case 'f':
				file = 1;
				fp = fopen(optarg, "r");
                                if (fp == NULL) {
					printf("my-look: cannot open file\n");
					exit(1);
				}
				break;
			case ':':
				printf("my-look: invalid command line\n");
				exit(1);
			case '?':
				printf("my-look: invalid command line\n");
				exit(1);
               }
        }

        //  Check if the command line only contains one specified string.
	if (optind != argc-1) {
            printf("my-look: invalid command line\n");
            exit(1);
        }

	/*
	 * This section deals with searching part.
	 * Search from the dictionary(file or stdin) to print out the lines with the specified prefix
	 */
	if (file == 0) {
		fp = stdin;  // The dictionary is from stdin.
	}
        char str[128];  // A buffer stores each line from dictionary.

	while(fgets(str, 128, fp) != NULL) {
		if(strncasecmp(str, argv[argc-1], strlen(argv[argc-1])) == 0) {
		       printf("%s", str);
	        }
        }
        fclose(fp);

	exit(0);
}
