#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
int main(){
    while(1){
        char argument_list[32];
        char input[32];
        char *commands[32];
        int i = 0;
        char *parameter_to_pass[32];
        char delimiter[] = " \n\t";
        printf("\ntype $ ");
        fgets(input, 32, stdin);
        input[strlen(input) - 1] = 0;
        char *token_command = strtok(input, delimiter);
        while(token_command != NULL){
            commands[i++] = token_command;
            token_command = strtok(NULL,delimiter);
        }
        commands[i++] = NULL;
        
        pid_t id = fork();
        if(id == 0){
            execvp(commands[0], commands);
        }
        else{
            wait(NULL);
            
        }
    }
}