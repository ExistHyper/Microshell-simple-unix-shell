#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
int main(){
    //use strtok
    while(1){
        char* argument_list[] = {"ls",NULL};
        char
        char commands[32];
        printf("\ntype $ ");
        fgets(commands, 32, stdin);
        strtok(commands,)
        commands[strlen(commands) - 1] = 0;
        pid_t id = fork();
        if(id == 0){
            execvp(commands, argument_list);
        }
        else{
            wait(NULL);
            printf("parent process\n");
        }
        printf("%s\n", commands);
    }
}