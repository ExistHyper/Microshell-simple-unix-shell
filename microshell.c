#include "microshell.h"

char arr_of_commands[][32] = {"hostname", "whoami", "exit", "pwd", "help", "cd", "cd ~", "cd ..", "cd /", "clear","bash","colors","color" };
char *commands[32];
char input[32];

int main(){
    int i  = 0;
    printf(RED "****WELCOME TO MICROSHELL****\n" RESET);
    printf("\n");

    while(1){
        char current_dir[PATH_MAX];
        getcwd(current_dir, PATH_MAX);
        printf("[%s] $ ",current_dir);
        fgets(input, 32, stdin);
        input[strlen(input) - 1] = 0;
        tokenization(input);
        fork_and_unknown_commands();
        get_hostname();
        get_login();
        get_current_dir();
        exit_function();
        help_option();
        my_cd();
        colors();
        color();
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
        commands[i++] = NULL;
    return;
}
void fork_and_unknown_commands(){
    int flag = 0;
    int status;
    for(int i = 0; i < 13; i++){
        if(strcmp(arr_of_commands[i], commands[0]) == 0)
            flag = 1;
    }
    if(flag == 0){
        int execvp_value;
        //pid_t id = fork();
        if(!fork())
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
        printf("List of commands:\n'exit'- leave microshell\n'cd' - change directory\n'pwd' - display current directory\n'whoami' - display login\n'hostname' - display hostname\n'colors' - change display colors\n");
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
    if(strcmp(commands[0],"colors") == 0 && commands[1] == NULL){
        int color;
        printf("To change colors use commands 'color {name of color in list}'\n");
        printf("List of available colors:\n1.black\n2.red\n3.green\n4.yellow\n5.blue\n6.purple\n7.cyan\n8.white\n9.reset\n");
    }else if(strcmp(commands[0],"colors") == 0 && commands[1] != NULL){
        printf("Too many arguments for function 'colors'\n");
    }
}
void color(){
    if(strcmp(commands[0],"color") == 0){
        if(commands[1] == NULL){
            printf("No color selected, please try again.\nFor help type 'colors'\n");
        }
        else if(strcmp(commands[1],"black") == 0 && commands[2] == NULL){
            printf(BLACK);
        }
        else if(strcmp(commands[1],"red") == 0 && commands[2] == NULL){
            printf(RED);
        }
        else if(strcmp(commands[1],"green") == 0 && commands[2] == NULL){
            printf(GREEN);
        }else if(strcmp(commands[1],"yellow") == 0   && commands[2] == NULL){
            printf(YELLOW);
        }
        else if(strcmp(commands[1],"blue") == 0 && commands[2] == NULL){
            printf(BLUE);
        }
        else if(strcmp(commands[1],"purple") == 0 && commands[2] == NULL){
            printf(PURPLE);
        }
        else if(strcmp(commands[1],"cyan") == 0 && commands[2] == NULL){
            printf(CYAN);
        }
        else if(strcmp(commands[1],"white") == 0 && commands[2] == NULL){
            printf(WHITE);
        }
        else if(strcmp(commands[1],"reset") == 0 && commands[2] == NULL){
            printf(RESET);
        }
        else{
            printf("Could not match color, please try again.\nFor help type 'colors'\n");
        }
    }
}

void clear(){
    if(strcmp(commands[0],"clear") == 0)
	    printf("\e[1;1H\e[2J");
//https://www.geeksforgeeks.org/clear-console-c-language/
}