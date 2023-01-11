#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

#define BLACK "\033[0;30m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"
#define RESET "\033[0m"

char arr_of_commands[][32] = {"hostname", "whoami", "exit", "pwd", "help", "cd", "cd ~", "cd ..", "cd /", "clear","bash","colors" };
char *commands[32];
char input[32];
void tokenization(char input_token[32]);
void fork_and_unknown_commands();
void get_hostname();
void get_login();
void get_current_dir();
void exit_function();
void help_option();
void my_cd();
void colors();
void clear();
int main(){
    int i  = 0;
    printf("****WELCOME TO MICROSHELL****\n");
    printf("\n");

    while(1){
        char current_dir[PATH_MAX];
        getcwd(current_dir, PATH_MAX);
        printf("[%s] $ " ,current_dir);
        fgets(input, 32, stdin);
        int num;
        printf("eNTER NUM: ");
        scanf("%d", &num);
        printf("num = %d",num);
        input[strlen(input) - 1] = 0;
        tokenization(input);
        colors();
        continue;
        fork_and_unknown_commands();
        get_hostname();
        get_login();
        get_current_dir();
        exit_function();
        help_option();
        my_cd();
        clear();
    }
}
void tokenization(char input_token[32]){
    int i = 0;
    char *token_command = strtok(input_token," \n\t");
    while(token_command != NULL){
        commands[i++] = token_command;
        token_command = strtok(NULL," \n\t");
    }
    //  CO TO JEST \N\T PAMIETAJ O TYM
        commands[i++] = NULL;
    return;
}
void fork_and_unknown_commands(){
    int flag = 0;
    int status;
    for(int i = 0; i < 12; i++){
        if(strcmp(arr_of_commands[i], commands[0]) == 0)
            flag = 1;
    }
    if(flag == 0){
        int execvp_value;
        pid_t id = fork();
        if(id == 0)
            execvp_value = execvp(commands[0], commands);
        else
            wait(NULL);
        if(execvp_value < 0){
            printf("Command not recognized.\nType 'help' for infornations\n");
            exit(1);     
        }
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

void exit_function(){
    if(strcmp(commands[0],"exit") == 0){
            exit(1);
    }
    
}

void get_current_dir(){
    if(strcmp(commands[0],"pwd") == 0){
        char current_dir[PATH_MAX];
        getcwd(current_dir, PATH_MAX);
        printf("%s\n",current_dir);
    }
    

}

void help_option(){
    if(strcmp(commands[0],"help") == 0){
        printf("List of commands:\n'exit'- leave microshell\ncd' - change directory\n'pwd' - display current directory\n'whoami' - display login\n'hostname' - display hostname\n");
        printf("Autor Jan Kordas\nWydzial informatyki i informatyki UAM Poznan\n");
    }
}

void my_cd(){
    char dir_before_chdir[PATH_MAX];
    getcwd(dir_before_chdir, PATH_MAX);
    if(strcmp(commands[0],"cd") == 0 && commands[1] == NULL){
        chdir(getenv("HOME"));
    }
    else if((strcmp(commands[0], "cd") == 0) && (strcmp(commands[1], "~") == 0)){
        chdir(getenv("HOME"));
    }
    else if((strcmp(commands[0], "cd") == 0) && (strcmp(commands[1], "..") == 0)){
        chdir("..");
    }
    else if((strcmp(commands[0], "cd") == 0) && (strcmp(commands[1], "/") == 0)){
        chdir("/");
    }
    else if(strcmp(commands[0], "cd") == 0 && commands[1] != NULL && commands[2] == NULL){
        chdir(commands[1]);
        char dir_after_chdir[PATH_MAX];
        getcwd(dir_after_chdir, PATH_MAX);
        if(strcmp(dir_before_chdir,dir_after_chdir) == 0)
            printf("No such directory\n");
    }else if(strcmp(commands[0], "cd") == 0 && commands[1] != NULL && commands[2] != NULL){
        printf("to many arguments\n");
    }
}
void colors (){
    if(strcmp(commands[0],"colors") == 0){
        int color;
        printf("List of available colors:\n1.Black\n2.Red\n3.Green\n4.Yellow\n5.Blue\n6.Purple\n7.Cyan\n8.White\n");
        printf("To change colors use commands 'color {name of color in list}'\n");
    }

}

void clear(){
    if(strcmp(commands[0],"clear") == 0)
	    printf("clear\n");
    
}