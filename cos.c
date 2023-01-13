#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
void handler(){
    exit(1);
}
int main(){
    printf("Hello");
    while(1){
        printf("Hello");
    }
    signal(SIGINT, handler);
    return 0;
}