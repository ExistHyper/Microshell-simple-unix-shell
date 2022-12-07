#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <signal.h>

char commands[40];
void get_current_dir();
void exit_function();
void unknown_command_error();
void help_option();
void display_files();

int main(){
    char hostname[_SC_HOST_NAME_MAX];
    hostname[39] = '\0';
    gethostname(hostname,40);
    printf("****WELCOME TO MICROSHELL****\n");
    printf("\n");

    while(1){
        printf("%s@%s:~$",getlogin(),hostname);
        fgets(commands, 40, stdin);
        
        
        get_current_dir();
        exit_function();
        unknown_command_error();
        help_option();
        display_files();

        //printf("%s", commands);
        //printf("\n%d\n", strcmp(commands,"exit"));
    }
}
void unknown_command_error(){
    if(strcmp(commands,"pwd") != 10 && strcmp(commands,"ls") != 10 && strcmp(commands,"exit") != 10 && strcmp(commands,"help") != 10){
        printf("command not found\ntype 'help' for informations\n");
    }
}

void exit_function(){
    if(strcmp(commands,"exit") == 10){
            exit(1);
    }
    
}

void get_current_dir(){
    if(strcmp(commands,"pwd") == 10){
        char current_dir[PATH_MAX];
        getcwd(current_dir, PATH_MAX);
        printf("%s\n",current_dir);
    }
    

}

void help_option(){
    if(strcmp(commands,"help") == 10){
        printf("Commands implemented:\n'exit'- leave microshell\n'pwd' - show current directory\n");
        printf("Autor Jan Kordas\nWydzial informatyki i informatyki UAM Poznan\n");
    }
}

void display_files(){
    if(strcmp(commands,"ls") == 10){
        DIR *dir;
        struct dirent *names_of_dirs;
        dir = opendir(".");
        if(dir == NULL){
            printf("Error in opening current dir");
        }
        while((names_of_dirs = readdir(dir)) != NULL){
            if((names_of_dirs -> d_type == DT_REG) || (names_of_dirs -> d_type == DT_DIR)){
                printf("%s\n", names_of_dirs -> d_name);
        }
    }
     closedir(dir);
}
}