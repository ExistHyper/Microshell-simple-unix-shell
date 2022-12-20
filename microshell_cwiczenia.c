#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
int main(){
    while(1){
        char input[32];
        char *commands[32];
        int i = 0;
        fgets(input, 32, stdin);
        input[strlen(input) - 1] = 0;
        char *token_command = strtok(input," \n\t");
        while(token_command != NULL){
            commands[i++] = token_command;
            token_command = strtok(NULL," \n\t");
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