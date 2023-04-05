#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <fcntl.h>

void get_arguments (char *line, int *argc, char *argv[]){
    char s[90000];
    strcpy (s, line);

    char *p = strtok (s, " ");
    int i=0;
    while(p!=NULL)
    {
        argv[i]= malloc(sizeof(p));
        strcpy(argv[i],p);
        i++;
        p = strtok (NULL, " ");
    }
    *argc=i;
    argv[i]=NULL;
}
void get_piped_commands (char *line, char *arrs[1000], int *nr_of_commands){
    char s[90000];
    strcpy (s, line);

    char *p = strtok (s, "|");
    int i=0;
    while(p!=NULL)
    {
        arrs[i]= malloc(sizeof(p));
        strcpy(arrs[i],p);
        i++;
        p = strtok (NULL, "|");
    }
    *nr_of_commands=i;
    arrs[i]=NULL;
}
int check_for_pipes (char *line) {
    for (int i=0; line[i]; i++)
        if (line[i]=='|')
            return 1;
    return 0;
}
int check_for_redirect (char *line, int *pos) {
    for (int i=0; line[i]; i++)
        if (line[i]=='>' && line[i+1]=='>')
        {
            *pos = i;
            return 2;
        }
        else if (line[i]=='>')
        {
            *pos = i;
            return 1;
        }
    return 0;
}
void split_redirect (char *line, int pos, int redirect_type, char *left, char *right) {
    strcpy(left, line);
    left[pos]='\0';
    int k;
    if (redirect_type==1)
        k=pos+1;
    else
        k=pos+2;
    while (line[k]==' ')
        k++;
    strcpy(right, line+k);
}

void execute_simple_command (char *arr[]){
    int pid = fork();
    if (pid==0)
    {
        if (strcmp (arr[0], "nl")==0)
            execv("my_nl", arr);
        else if (strcmp(arr[0], "head")==0)
            execv("my_head", arr);
        else if (strcmp(arr[0], "chroot")==0)
            execv("my_chroot", arr);
        else if (strcmp(arr[0], "help")==0)
            execv("my_help", arr);
        else if (strcmp(arr[0], "version")==0)
            execv("my_version", arr);
        else
        if (strcmp(arr[0], "exit")!=0)
            execvp (arr[0], arr);

        printf("No such command\n");
        exit(1);
    }
    else if (pid<0)
    {
        printf("Error creating child process\n");
        exit(2);
    }
    else
    {
        wait(NULL);
        return ;
    }
}
void execute_piped_commands (char *arr[], int nr_of_commands){
    int pid = fork();
    if (pid==0)
    {
        int redirect = 0, type_of_redirect;
        char *file_name_redirect;
        int argc;
        char *arrs[1000][300];
        for (int i=0; i<nr_of_commands; i++)
        {
            if (i==nr_of_commands-1)
            {
                //without pipes
                int pos_r=0;
                int type_redirect = check_for_redirect(arr[i], &pos_r);
                if (type_redirect!=0)
                {
                    //redirect
                    char *left, *right;
                    left = malloc(sizeof(strlen(arr[i])));
                    right = malloc(sizeof(strlen(arr[i])));
                    split_redirect(arr[i], pos_r, type_redirect, left, right);

                    int nr_of_arg=0;
                    get_arguments(left, &nr_of_arg, arrs[i]);

                    redirect=1;
                    type_of_redirect=type_redirect;
                    file_name_redirect = malloc(sizeof (right));
                    strcpy(file_name_redirect, right);
                }
                else
                    get_arguments(arr[i], &argc, arrs[i]);
            }
            else
                get_arguments(arr[i], &argc, arrs[i]);
        }

        int nr_of_pipes = nr_of_commands - 1;
        int pipes[nr_of_commands][2];

        for (int i=0; i<nr_of_pipes; i++)
            if (pipe(pipes[i])==-1)
            {
                printf("Error creating pipe\n");
                exit(3);
            }

        int pid_i = fork();
        if (pid_i<0)
        {
            printf("Error creating child process\n");
            exit(2);
        }
        else if (pid_i==0)
        {
            close(pipes[0][0]);
            dup2(pipes[0][1], 1);

            if (strcmp (arrs[0][0], "nl")==0)
                execv("my_nl", arrs[0]);
            else if (strcmp(arrs[0][0], "head")==0)
                execv("my_head", arrs[0]);
            else if (strcmp(arrs[0][0], "chroot")==0)
                execv("my_chroot", arrs[0]);
            else
                execvp(arrs[0][0], arrs[0]);

            printf("No such command\n");
            exit(1);
        }
        else
        {
            close(pipes[0][1]);
            for (int i=1; i<nr_of_commands; i++)
            {
                int pid_ii = fork();
                if (pid_ii<0)
                {
                    printf("Error creating child process\n");
                    exit(2);
                }
                else if (pid_ii==0)
                {
                    close(pipes[i][0]);
                    dup2(pipes[i-1][0], 0);
                    dup2(pipes[i][1], 1);
                    close(pipes[i-1][0]);
                    close(pipes[i][1]);

                    if (redirect==1 && i==nr_of_commands-1)
                    {
                        int terminal_fd = dup(STDOUT_FILENO);
                        int fd;
                        if (type_of_redirect==1)
                            fd = open(file_name_redirect, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                        else
                            fd = open (file_name_redirect, O_WRONLY | O_CREAT | O_APPEND, 0644);

                        dup2(fd, STDOUT_FILENO);
                        close(fd);

                        if (strcmp (arrs[i][0], "nl")==0)
                            execv("nl", arrs[i]);
                        else if (strcmp(arrs[i][0], "head")==0)
                            execv("my_head", arrs[i]);
                        else if (strcmp(arrs[i][0], "chroot")==0)
                            execv("my_chroot", arrs[i]);
                        else
                            execvp(arrs[i][0], arrs[i]);

                        dup2(terminal_fd, STDOUT_FILENO);
                        close(terminal_fd);
                    }
                    else
                    {
                        if (strcmp (arrs[i][0], "nl")==0)
                            execv("nl", arrs[i]);
                        else if (strcmp(arrs[i][0], "head")==0)
                            execv("my_head", arrs[i]);
                        else if (strcmp(arrs[i][0], "chroot")==0)
                            execv("my_chroot", arrs[i]);
                        else
                            execvp(arrs[i][0], arrs[i]);

                        printf("No such command\n");
                        exit(1);
                    }
                }
                else
                {
                    for (int j=0; j<nr_of_commands-1; j++) {
                        close(pipes[j][1]);
                    }
                    wait(NULL);
                    close(pipes[i][1]);
                }
            }
            wait(NULL);
        }
        exit(0);
    }

    else if (pid<0)
    {
        printf("Error creating child process\n");
        exit(2);
    }
    else
    {
        wait(NULL);
        return ;
    }

}

int main() {
    char *line = readline("iulia_t>> ");
    add_history(line);
    while (strcmp(line, "exit")!=0){
        if (check_for_pipes(line) == 1)
        {
            //pipes
            char *commands[1000];
            int nr_of_commands;
            get_piped_commands(line, commands, &nr_of_commands);
            execute_piped_commands(commands, nr_of_commands);
        }
        else
        {
            //without pipes
            int pos_r=0;
            int type_of_redirect = check_for_redirect(line, &pos_r);
            if (type_of_redirect!=0)
            {
                //redirect
                char *left, *right;
                left = malloc(sizeof(strlen(line)));
                right = malloc(sizeof(strlen(line)));
                split_redirect(line, pos_r, type_of_redirect, left, right);

                char *args[300];
                int argc=0;
                get_arguments(left, &argc, args);

                int terminal_fd = dup(STDOUT_FILENO);
                int fd;
                if (type_of_redirect==1)
                    fd = open(right, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                else
                    fd = open (right, O_WRONLY | O_CREAT | O_APPEND, 0644);

                dup2(fd, STDOUT_FILENO);
                close(fd);

                if (strcmp(args[0], "exit")!=0)
                    execute_simple_command(args);

                dup2(terminal_fd, STDOUT_FILENO);
                close(terminal_fd);
            }
            else
            {
                //without redirect
                char *args[300];
                int argc=0;
                get_arguments(line, &argc, args);

                if (strcmp(args[0], "exit")!=0)
                    execute_simple_command(args);
            }
        }
        free(line);
        line = readline ("iulia_t>> ");
        add_history(line);
    }
    return 0;
}
