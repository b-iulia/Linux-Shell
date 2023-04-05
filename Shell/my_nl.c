#include "my_nl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

int pos_s=-1, pos_d=-1, nr_files=0;
char string_s[1000], string_d[10];
char *flname[100];

int check_for_arguments (char *argv[]) {
    strcpy(string_s, "");
    strcpy(string_d, "");
    pos_d=-1, pos_d=-1;
    nr_files=0;
    for (int i=0; i<100; i++)
        flname[i]=NULL;
    for (int i=1; argv[i]; i++)
    {
        if (strcmp(argv[i], "-s")==0 && strcmp(argv[i-1], "-s")!=0 && strcmp(argv[i-1], "-d")!=0)
        {
            pos_s=i;
            if (argv[i+1]!=NULL)
                strcpy(string_s, argv[i+1]);
            else
            {
                printf("nl: option requires an argument -- 's'\n");
                return 1;
            }
        }
        else if (strcmp(argv[i], "-d")==0 && strcmp(argv[i-1], "-s")!=0 && strcmp(argv[i-1], "-d")!=0)
        {
            pos_d=i;
            if (argv[i+1]!=NULL)
            {
                if (strlen(argv[i+1])!=1 && strlen(argv[i+1])!=2)
                {
                    printf("d (delimiter) option requires argument of 1 or 2 characters\n");
                    return 1;
                }
                strcpy(string_d, argv[i+1]);
                if (strlen(argv[i+1])==1)
                {
                    strcat(string_d, ":");
                    string_d[strlen(string_d)]='\0';
                }

            }
            else
            {
                printf("nl: option requires an argument -- 'd'\n");
                return 1;
            }
        }
        else if (strcmp(argv[i-1], "-s")!=0 && strcmp (argv[i-1], "-d")!=0)
        {
            flname[nr_files] = malloc(sizeof(argv[i]));
            strcpy (flname[nr_files], argv[i]);
            nr_files++;
        }
    }
    return 0;
}
void print_alignment (int i) {
    if (i<=9)
        printf("     ");
    else if (i>9 && i<=99)
        printf("    ");
    else if (i>99 && i<=999)
        printf("   ");
    else if (i>999 && i<=9999)
        printf("  ");
    else if (i>9999 && i<=99999)
        printf(" ");
}
void print_line (int *numbered, int *i, char *line, char s[], int d) {
    if (strcmp(line, "\\:\\:\\:\n")==0)
    {
        *numbered=0;
        if (d==1)
            printf("%s%s%s\n", string_d, string_d, string_d);
        else
            printf("\n");
    }
    else if (strcmp(line, "\\:\\:\n")==0)
    {
        *numbered=1;
        *i=1;
        if (d==1)
            printf("%s%s\n", string_d, string_d);
        else
            printf("\n");
    }
    else if (strcmp(line, "\\:\n")==0)
    {
        *numbered=0;
        if (d==1)
            printf("%s\n", string_d);
        else
            printf("\n");
    }

    else if (strcmp(line, "\n")==0)
        printf("%s", line);
    else if (*numbered==1)
    {
        print_alignment(*i);
        printf("%d%s%s", *i, s, line);
        (*i)++;
    }
    else
    {
        print_alignment(*i);
        printf("%s", line);
    }
}
void display_file (char s[], int d) {
    FILE *fp[nr_files];
    fp[0]=NULL;
    int k=1;
    int numbered=1;
    for (int i=0; i<nr_files; i++)
    {
        fp[i] = fopen(flname[i], "r");
        if (fp[i]==NULL)
        {
            perror("Error opening file");
            return ;
        }
        else
        {
            char line[1000];
            while (fgets(line, 1000, fp[i])!=NULL)
                print_line(&numbered, &k, line, s, d);

            fclose(fp[i]);
        }
    }
}

int my_nl(int argc, char *argv[]) {

    pos_s=-1;
    pos_d=-1;
    int a=check_for_arguments(argv);
    if (a==1)
        return 1;
    if (nr_files==0)
    {
        if ((pos_s==-1 && pos_d==-1) || (pos_d!=-1 && pos_s==-1))
        {
            char *line= readline ("");
            int i=1;
            int numbered=1;
            while (strcmp(line, "exit")!=0)
            {
                print_alignment(i);
                strcat(line, "\n");
                line[strlen(line)]='\0';
                print_line(&numbered, &i, line, " ", 0);

                free(line);
                line = readline ("");
            }
            return 0;
        }
        else if (pos_s!=-1)
        {
            int d=0;
            if (pos_d!=-1)
                d=1;
            char *line= readline ("");
            int i=1;
            int numbered=1;
            while (strcmp(line, "exit")!=0)
            {
                print_alignment(i);
                strcat(line, "\n");
                line[strlen(line)]='\0';
                print_line(&numbered, &i, line, string_s, d);

                free(line);
                line = readline ("");
            }
            return 0;
        }
    }
    else
    {
        if (argc==2 && nr_files==1)
        {
            display_file(" ", 0);
            return 0;
        }
        else
        {
            int in=0;

            if (pos_s!=-1)
            {
                int d=0;
                if (pos_d!=-1)
                    d=1;
                display_file(string_s, d);
                in=1;
            }
            else if (pos_s==-1 && pos_d!=-1)
            {
                display_file(" ", 1);
                in=1;
            }
            else if (pos_s==-1 && pos_d==-1)
            {
                display_file(" ", 0);
                in=1;
            }

            if (in==1)
                return 0;
            else
            {
                printf("Wrong command\n");
                return 1;
            }
        }
    }
    return 0;
}
int main(int argc, char *argv[]) {
    int r = my_nl(argc, argv);
    return r;
}