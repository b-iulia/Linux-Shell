#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc==1)
    {
        printf("HELP\n----------------\n");
        printf("chroot command: Changes the root directory to a new directory\n\n\n");
        printf("head command: Prints the first 10 lines of each file to standard output\nIf there are multiple files, before the content of each file it will be printed the file's name.\n\n      -n (lines) NUMBER --- prints the first NUMBER lines of the given file(s)\n      -c (bytes) NUMBER --- prints the first NUMBER bytes of the given file(s)\n-v (verbose) --- prints the name of the file before its content (even if it is just one file)\n-q (quiet) --- does not print the file names even if there are multiple files\n\n\n");
        printf("nl command: Writes the content of the file to standard output, with the line numbers added.\n      -s (separator) SEPARATOR --- separates the text from its line number with the character specified in the SEPARATOR variable\n      -d (section delimiter) DELIMITER --- uses the 2 characters specified in the DELIMITER variable as the delimiters for the start of a logical page section. If the user enters only one character, the second character used will be a colon(:)\n");
        return 0;
    }
    else if (argc==2)
    {
        if (strcmp(argv[1], "chroot")==0)
        {
            printf("\nchroot command: Changes the root directory to a new directory\n");
            return 0;
        }
        else if (strcmp(argv[1], "head")==0)
        {
            printf("\nhead command: Prints the first 10 lines of each file to standard output\nIf there are multiple files, before the content of each file it will be printed the file's name.\n\n-n (lines) NUMBER --- prints the first NUMBER lines of the given file(s)\n-c (bytes) NUMBER --- prints the first NUMBER bytes of the given file(s)\n-v (verbose) --- prints the name of the file before its content (even if it is just one file)\n-q (quiet) --- does not print the file names even if there are multiple files\n");
            return 0;
        }
        else if (strcmp(argv[1], "nl")==0)
        {
            printf("\nnl command: Writes the content of the file to standard output, with the line numbers added.\n\n-s (separator) SEPARATOR --- separates the text from its line number with the character specified in the SEPARATOR variable\n-d (section delimiter) DELIMITER --- uses the 2 characters specified in the DELIMITER variable as the delimiters for the start of a logical page section. If the user enters only one character, the second character used will be a colon(:)\n");
            return 0;
        }
        else
        {
            printf("No such command\n");
            return 1;
        }
    }
    else
    {
        printf("No such command\n");
        return 1;
    }
}