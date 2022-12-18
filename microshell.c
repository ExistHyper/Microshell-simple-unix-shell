#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

char arr_of_commands[][32] = {"hostname", "whoami", "exit", "pwd", "help", "cd", "cd ~", "cd ..", "cd /", "clear","bash" };
char commands[32];
void get_hostname();
void get_login();
void get_current_dir();
void exit_function();
void unknown_command_error();
void help_option();
void my_cd();
void clear();
void bash();


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
        my_cd();
        clear();
        bash();

    }
}
void get_hostname(){
    if(strcmp(commands,"hostname") == 0){
    char hostname[_SC_HOST_NAME_MAX];
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
    for(int i = 0; i < 11; i++){
        if(strcmp(arr_of_commands[i], commands) != 0)
            printf("Command not recognized.\nType 'help' for infornations\n");
    }
}
void bash(){
    for(int i = 0; i < 11; i++){
        if(strcmp(commands, arr_of_commands[i]) != 0){
            pid_t id = fork();
            if(id == 0){
                execvp(commands)
            }
            else{
                wait(NULL);
                printf("We re back\n");
                } 
        }
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
        printf(" List of commands:\n'exit'- leave microshell\n'pwd' - display current directory\n'whoami' - display login\n'hostname' - display hostname");
        printf("Autor Jan Kordas\nWydzial informatyki i informatyki UAM Poznan\n");
    }
}

void my_cd(){
    if(strcmp(commands,"cd") == 0){
        chdir(getenv("HOME"));
    }
    else if(strcmp(commands, "cd ~") == 0){
        chdir(getenv("HOME"));
    }
    else if(strcmp(commands, "cd ..") == 0){
        chdir("..");
    }
    else if(strcmp(commands, "cd /") == 0){
        chdir("/");
    }

    }

void clear(){
    if(strcmp(commands,"clear") == 0)
	    printf("clear\n");
    
}