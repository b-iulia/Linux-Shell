#include "my_head.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <unistd.h>

char* fname[100];
char string_n[10]="", string_c[10000]="";
int q=0, v=0, n=0, c=0, nr_of_files=0, pos_q=-1, pos_v=-1, pos_n=-1, pos_c=-1;
void print_from_keyboard_lines (int x) {
    for (int i=0; i<x; i++)
    {
        char line[10000];
        fgets(line, 10000, stdin);
        printf("%s", line);

        fflush(stdout);
    }
}
void print_from_keyboard_bytes (int x) {
    int k=0;
    while (k<x)
    {
        char *line = readline("");
        int length = strlen(line);
        if (k+length <= x)
        {
            printf("%s", line);
            k+=length;
        }
        else
        {
            for (int j=0; j+1+k<=x; j++)
                printf("%c", line[j]);
            k=x+1;
        }
        free(line);
    }
}
void print_num_lines (int num) {
    FILE *fp[nr_of_files];
    fp[0]=NULL;
    for (int i=0; i<nr_of_files; i++)
    {
        fp[i] = fopen (fname[i], "r");
        if (fp[i]==NULL)
        {
            perror("Error opening file");
            return ;
        }
        else
        {
            char line[1000];
            int k=0;
            while (fgets(line, 1000, fp[i])!=NULL && k<num)
            {
                printf("%s", line);
                k++;
            }

            fflush(stdout);
            fclose(fp[i]);
        }
    }
}
void print_num_lines_with_names (int num) {
    FILE *fp[nr_of_files];
    fp[0]=NULL;
    for (int i=0; i<nr_of_files; i++)
    {
        fp[i] = fopen (fname[i], "r");
        if (fp[i]==NULL)
        {
            perror("Error opening file");
            return ;
        }
        else
        {
            if (i>0)
                printf("\n");
            printf("==> %s <==\n", fname[i]);
            char line[1000];
            int k=0;
            while (fgets(line, 1000, fp[i])!=NULL && k<num)
            {
                printf("%s", line);
                k++;
            }

            fflush(stdout);
            fclose(fp[i]);
        }
    }
}
void print_num_bytes (int num) {
    FILE *fp[nr_of_files];
    fp[0]=NULL;
    for (int i=0; i<nr_of_files; i++)
    {
        fp[i] = fopen (fname[i], "r");
        if (fp[i]==NULL)
        {
            perror("Error opening file");
            return ;
        }
        else
        {
            char line[1000];
            int k=0;
            while (fgets(line, 1000, fp[i])!=NULL && k<num)
            {
                int length = strlen(line);
                if (k+length <= num)
                {
                    printf("%s", line);
                    k+=length;
                }
                else
                {
                    for (int j=0; j+1+k<=num; j++)
                        printf("%c", line[j]);
                    k=num+1;
                }
            }

            fflush(stdout);
            fclose(fp[i]);
        }
    }
}
void print_num_bytes_with_names (int num) {
    FILE *fp[nr_of_files];
    fp[0]=NULL;
    for (int i=0; i<nr_of_files; i++)
    {
        fp[i] = fopen (fname[i], "r");
        if (fp[i]==NULL)
        {
            perror("Error opening file");
            return ;
        }
        else
        {
            if (i>0)
                printf("\n");
            printf("==> %s <==\n", fname[i]);
            char line[1000];
            int k=0;
            while (fgets(line, 1000, fp[i])!=NULL && k<num)
            {
                int length = strlen(line);
                if (k+length <= num)
                {
                    printf("%s", line);
                    k+=length;
                }
                else
                {
                    for (int j=0; j+1+k<=num; j++)
                        printf("%c", line[j]);
                    k=num+1;
                }
            }

            fflush(stdout);
            fclose(fp[i]);
        }
    }
}

int check_for_args (char *argv[]) {
    strcpy(string_n, "");
    strcpy(string_c, "");
    q=0, v=0, n=0, c=0, nr_of_files=0;
    pos_q=-1, pos_v=-1, pos_n=-1, pos_c=-1;
    for (int i=0; i<100; i++)
        fname[i]=NULL;
    nr_of_files=0;
    for (int i=1; argv[i]; i++)
    {
        if (strcmp(argv[i-1], "-n")==0)
        {
            if (argv[i]==NULL || strcmp(argv[i], "-c")==0 || strcmp(argv[i], "-q")==0 || strcmp(argv[i], "-v")==0)
            {
                printf("head: option requires an argument -- 'n'\n");
                return 1;
            }
            else
            {
                strcpy(string_n, argv[i]);
                n=1;
                pos_n=i-1;
            }
        }
        else if (strcmp(argv[i-1], "-c")==0)
        {
            if (argv[i]==NULL || strcmp(argv[i], "-n")==0 || strcmp(argv[i], "-q")==0 || strcmp(argv[i], "-v")==0)
            {
                printf("head: option requires an argument -- 'c'\n");
                return 1;
            }
            else
            {
                strcpy(string_c, argv[i]);
                c=1;
                pos_c=i-1;
            }
        }
        else if (strcmp(argv[i], "-q")==0)
        {
            q=1;
            pos_q=i;
        }
        else if (strcmp(argv[i], "-v")==0)
        {
            v=1;
            pos_v=i;
        }
        else if (strcmp(argv[i], "-n")==0)
            n=1;
        else if (strcmp(argv[i], "-c")==0)
            c=1;
        else if (strcmp(argv[i], "-n")!=0 && strcmp(argv[i], "-c")!=0)
        {
            fname[nr_of_files]= malloc(sizeof(argv[i]));
            strcpy(fname[nr_of_files], argv[i]);
            nr_of_files++;
        }
    }
    if (n==1 && pos_n==-1)
    {
        printf("head: option requires an argument -- 'n'\n");
        return 1;
    }
    if (c==1 && pos_c==-1)
    {
        printf("head: option requires an argument -- 'c'\n");
        return 1;
    }
    return 0;
}
void check_q_v_lines (int num) {
    if (q==1 && v==1)
    {
        if (pos_q > pos_v)
            print_num_lines(num);
        else
            print_num_lines_with_names(num);
    }
    else if (q==1 || (nr_of_files==1 && v==0))
        print_num_lines(num);
    else if (v==1 || (nr_of_files>1 && q==0))
        print_num_lines_with_names(num);
    else
        print_num_lines(num);
}

int my_head(int argc, char *argv[]) {
    if (argc==1)
    {
        print_from_keyboard_lines(10);
        return 0;
    }
    else
    {
        int x=check_for_args(argv);
        if (x==1)
            return 1;
        if (nr_of_files==0)
        {
            if (n==0 && c==0)
            {
                if ((v==1 && q==0) || (v==1 && q==1 && pos_v > pos_q))
                    printf("==> standard input <==\n");
                print_from_keyboard_lines(10);
            }
            else if ((n==1 && c==1 && pos_n > pos_c) || (n==1 && c==0))
            {
                int num = atoi(string_n);
                if ((v==1 && q==0) || (v==1 && q==1 && pos_v > pos_q))
                    printf("==> standard input <==\n");
                print_from_keyboard_lines(num);
            }
            else if ((n==1 && c==1 && pos_c > pos_n) || (c==1 && n==0))
            {
                int num = atoi(string_c);
                if ((v==1 && q==0) || (v==1 && q==1 && pos_v > pos_q))
                    printf("==> standard input <==\n");
                print_from_keyboard_bytes(num);
            }
            else
            {
                if ((v==1 && q==0) || (v==1 && q==1 && pos_v > pos_q))
                    printf("==> standard input <==\n");
                print_from_keyboard_lines(10);
            }
            return 0;
        }
        else
        {
            if (n==0 && c==0)
                check_q_v_lines(10);
            else if ((n==1 && c==1 && pos_n > pos_c) || (n==1 && c==0))
            {
                int num = atoi(string_n);
                check_q_v_lines(num);
            }
            else if ((n==1 && c==1 && pos_c > pos_n) || (c==1 && n==0))
            {
                int num = atoi(string_c);
                if (q==1 && v==1)
                {
                    if (pos_q > pos_v)
                        print_num_bytes(num);
                    else
                        print_num_bytes_with_names(num);
                }
                else if (q==1 || (nr_of_files==1 && v==0))
                    print_num_bytes(num);
                else if (v==1 || (nr_of_files>1 && q==0))
                    print_num_bytes_with_names(num);
                else
                    print_num_bytes(num);
            }
            else
                check_q_v_lines(10);
            return 0;
        }
    }
}
int main(int argc, char *argv[]) {
    int r = my_head(argc, argv);
    return r;
}