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
char *commands[32];
char input[32];
int fork_value;
void get_hostname();
void get_login();
void get_current_dir();
void exit_function();
void unknown_command_error();
void help_option();
void my_cd();
void clear();
void tokenization(char input_token[32]){
    int i = 0;
    char *token_command = strtok(input_token," \n\t");
        while(token_command != NULL){
            commands[i++] = token_command;
            token_command = strtok(NULL," \n\t");
        }
        commands[i++] = NULL;
    return;
}
void fork_commands(){
    int flag = 0;
        for(int i = 0; i < 11; i++){
            if(strcmp(arr_of_commands[i], commands[0]) == 0){
                flag = 1;
            }
        }
        if(flag == 0){
            pid_t id = fork();
            if(id == 0){
                fork_value = execvp(commands[0], commands);
            }else{
                wait(NULL);
        }
        }
        
}

int main(){
    int i  = 0;
    printf("****WELCOME TO MICROSHELL****\n");
    printf("\n");

    while(1){
        char current_dir[PATH_MAX];
        getcwd(current_dir, PATH_MAX);
        printf("[%s] $ ", current_dir);
        fgets(input, 32, stdin);
        input[strlen(input) - 1] = 0;
        tokenization(input);

        fork_commands();
        get_hostname();
        get_login();
        get_current_dir();
        exit_function();
        unknown_command_error();
        help_option();
        my_cd();
        clear();
        printf("\nfork value: %d\n",fork_value);
    
        
    }
}
void get_hostname(){
    if(strcmp(commands[0],"hostname") == 0){
    char hostname[_SC_HOST_NAME_MAX];
    gethostname(hostname,40);
    printf("%s\n", hostname); 
    }
    
}
void get_login(){
    if(strcmp(commands[0],"whoami") == 0){
        printf("%s\n",getlogin());
    }
}

void unknown_command_error(){
    int flag = 0;
    for(int i = 0; i < 11; i++){
        if(strcmp(arr_of_commands[i], commands[0]) != 0 && fork_value < 0)
            flag = 1;
    }
    if(flag == 1)
        printf("Command not recognized.\nType 'help' for infornations\n");
}

void exit_function(){
    if(strcmp(commands[0],"exit") == 0){
            exit(1);
    }
    
}

void get_current_dir(){
    if(strcmp(commands[0],"pwd") == 0){
        char current_dir[PATH_MAX];
        getcwd(current_dir, PATH_MAX);
        printf("my pwd: %s\n",current_dir);
    }
    

}

void help_option(){
    if(strcmp(commands[0],"help") == 0){
        printf(" List of commands:\n'exit'- leave microshell\n'pwd' - display current directory\n'whoami' - display login\n'hostname' - display hostname");
        printf("Autor Jan Kordas\nWydzial informatyki i informatyki UAM Poznan\n");
    }
}

void my_cd(){
    if(strcmp(commands[0],"cd") == 0){
        chdir(getenv("HOME"));
    }
    else if(strcmp(commands[0], "cd ~") == 0){
        chdir(getenv("HOME"));
    }
    else if(strcmp(commands[0], "cd ..") == 0){
        chdir("..");
    }
    else if(strcmp(commands[0], "cd /") == 0){
        chdir("/");
    }

    }

void clear(){
    if(strcmp(commands[0],"clear") == 0)
	    printf("clear\n");
    
}