#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <signal.h>

char arr_of_commands[][32] = {"hostname", "whoami", "exit", "pwd", "help", "ls" };
char commands[32];
void get_hostname();
void get_login();
void get_current_dir();
void exit_function();
void unknown_command_error();
void help_option();
void display_files();



int main(){
    printf("****WELCOME TO MICROSHELL****\n");
    printf("\n");

    while(1){
        char current_dir[PATH_MAX];
        getcwd(current_dir, PATH_MAX);
        printf("[%s] $ ", current_dir);
        fgets(commands, 32, stdin);
        commands[strlen(commands) - 1] = 0; //usuwanie znaku konca stringu
        


        get_hostname();
        get_login();
        get_current_dir();
        exit_function();
        unknown_command_error();
        help_option();
        display_files();

    }
}
void get_hostname(){
    if(strcmp(commands,"hostname") == 0){
    char hostname[_SC_HOST_NAME_MAX];
    //hostname[39] = '\0';
    gethostname(hostname,40);
    printf("%s\n", hostname); 
    }
    
}
void get_login(){
    if(strcmp(commands,"whoami") == 0){
        printf("%s\n",getlogin());
    }
}

void unknown_command_error(){
    int flag = 0;
    for(int i = 0; i < 6; i++){
        if(strcmp(arr_of_commands[i], commands) == 0){
            flag = 1;
        }
    }
    if(flag == 0){
        printf("command not found\ntype 'help' for informations\n");
    }
    
}

void exit_function(){
    if(strcmp(commands,"exit") == 0){
            exit(1);
    }
    
}

void get_current_dir(){
    if(strcmp(commands,"pwd") == 0){
        char current_dir[PATH_MAX];
        getcwd(current_dir, PATH_MAX);
        printf("%s\n",current_dir);
    }
    

}

void help_option(){
    if(strcmp(commands,"help") == 0){
        printf("List of commands:\n
        'exit'- leave microshell\n'
        'pwd' - display current directory\n
        'whoami' - display login
        'hostname' - display hostname
        
        ");
        printf("Autor Jan Kordas\nWydzial informatyki i informatyki UAM Poznan\n");
    }
}

void display_files(){
    if(strcmp(commands,"ls") == 0){
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
