#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define BUFSIZ 128

int main(int argc, char *argv[]){
    char buf[BUFSIZ];
    int f,i,n;

    if(argc <= 1)
        while ((n = read(0, buf, BUFSIZ)) > 0)
            write(1, buf, n);

    for(i=1; i<argc; i++){
        if((f = open(argv[i], O_RDONLY)) > 0){
            while ((n = read(f, buf, BUFSIZ)) > 0)
                write(1, buf, n);
            close(f);
        } else {
            perror(argv[i]);
        }
    }
    return 0;
}