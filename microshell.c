#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>
#include <fcntl.h>

#define SIZE 512

#define BLACK "\033[0;30m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"
#define RESET "\033[0m"

void tokenization(char input_token[SIZE]);
void handle_cp_mv_fork();
void fork_and_unknown_commands();
void get_hostname();
void get_login();
void get_current_dir();
void exit_function();
void help_option();
void my_cd();
void colors();
void color();
void clear();
void my_mv_files();
void my_cp_files();
int gethostname(char *name, size_t len);

char arr_of_commands[][32] = {
    "hostname",
    "whoami",
    "exit",
    "pwd",
    "help",
    "cd",
    "clear",
    "bash",
    "colors",
    "color",
};
char *commands[SIZE];
char input[SIZE];
int arguments_counter = 0;
char dir_before_chdir[PATH_MAX] = {0};

int main() {
    printf(RED "****WELCOME TO MICROSHELL****\n"
        RESET);
    printf("\n");

    while (1) {
        char current_dir[PATH_MAX];
        char prompt_hostname[_SC_HOST_NAME_MAX];
        char input[SIZE];
        for (int i = 0; i < SIZE; i++) {
            commands[i] = 0;
        }
        gethostname(prompt_hostname, 40);
        getcwd(current_dir, PATH_MAX);
        printf("%s@%s:[%s] $ ", getlogin(), prompt_hostname, current_dir);
        do {
            fgets(input, SIZE, stdin);
            if (strlen(input) <= 1) {
                printf("%s@%s:[%s] $ ", getlogin(), prompt_hostname, current_dir);
            }
        } while (strlen(input) <= 1);
        input[strlen(input) - 1] = 0;
        tokenization(input);
        if (arguments_counter == -1) {
            continue;
        }
        handle_cp_mv_fork();
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
    return 0;
}

void tokenization(char input_token[SIZE]) {
    char * token_command = {0};
    int i = 0;
    token_command = strtok(input_token, " \t");
    while (token_command) {
        commands[i++] = token_command;
        token_command = strtok(NULL, " \t");
    }
    arguments_counter = i - 1;
    commands[i++] = NULL;
    return;
}
void handle_cp_mv_fork() {
    if (arguments_counter >= 1) {
        if (!strcmp(commands[0], "mv") && !strcmp(commands[1], "-my")) {
            my_mv_files();
        } else if (!strcmp(commands[0], "cp") && !strcmp(commands[1], "-my")) {
            my_cp_files();
        } else if ((strcmp(commands[0], "cp") || strcmp(commands[0], "mv")) && !strcmp(commands[1], "-my")) {
            printf("Command not recognized.\nDid you mean 'mv -my' or 'cp -my'?\n");
        } else {
            fork_and_unknown_commands();
            return;
        }
    } else {
        fork_and_unknown_commands();
        return;
    }
}
void fork_and_unknown_commands() {
    for (int i = 0; i < 10; i++) {
        if (arguments_counter > 1) {
            if (!strcmp(arr_of_commands[i], commands[0]) || !strcmp(arr_of_commands[i], commands[1])) {
                return;
            }
        } else {
            if (!strcmp(arr_of_commands[i], commands[0])) {
                return;
            }
        }
    }
    int execvp_value = 0;
    if (!fork()) {
        execvp_value = execvp(commands[0], commands);
    } else {
        wait(NULL);
    }
    if (execvp_value < 0) {
        printf("Command not recognized.\nType 'help' for infornations\n");
        exit(1);
    }
    return;
}

void get_hostname() {
    char hostname[_SC_HOST_NAME_MAX];
    if (!strcmp(commands[0], "hostname")) {
        gethostname(hostname, _SC_HOST_NAME_MAX);
        printf("%s\n", hostname);
    }
    return;
}

void get_login() {
    if (!strcmp(commands[0], "whoami")) {
        printf("%s\n", getlogin());
    }
    return;
}

void exit_function() {
    if (!strcmp(commands[0], "exit")) {
        exit(1);
    }
    return;
}

void get_current_dir() {
    char current_dir[PATH_MAX];
    if (!strcmp(commands[0], "pwd")) {
        getcwd(current_dir, PATH_MAX);
        printf("%s\n", current_dir);
    }
    return;
}

void help_option() {
    if (!strcmp(commands[0], "help")) {
        printf("\n           List of commands         \n");
        printf("-------------------------------------------- \n");
        printf("    'exit'    | - leave microshell\n    'cd'      | - change directory\n    'mv -my'  | - rename file\n    'cp -my'  | - copy file\n    'pwd'     | - display current directory\n    'whoami'  | - display login\n    'hostname'| - display hostname\n    'colors'  | - change display colors\n");
        printf("--------------------------------------------\n");
        printf("Autor Jan Kordas\nWydzial informatyki i informatyki UAM Poznan\n");
    }
    return;
}

void my_cd() {
    char dir_after_chdir[PATH_MAX];
    char dir_before_chdir_backup[PATH_MAX];

    if (!strcmp(commands[0], "cd") && !commands[1]) {
        getcwd(dir_before_chdir, PATH_MAX);
        chdir(getenv("HOME"));
    } else if (!strcmp(commands[0], "cd") && commands[1] && commands[2]) {
        printf("Too many arguments\n");
    } else if (!strcmp(commands[0], "cd") && (!strcmp(commands[1], "~") || !strcmp(commands[1], "."))) {
        getcwd(dir_before_chdir, PATH_MAX);
        chdir(getenv("HOME"));
    } else if (!strcmp(commands[0], "cd") && !strcmp(commands[1], "-")) {
        for (int i = 0; i < PATH_MAX; i++){
            dir_before_chdir_backup[i] = 0;
        }
        if (!dir_before_chdir[1]) {
            printf("Previous directory not set\n");
        } else {
            memcpy(dir_before_chdir_backup, dir_before_chdir, strlen(dir_before_chdir));
            getcwd(dir_before_chdir, PATH_MAX);
            printf("%s\n", dir_before_chdir_backup);
            chdir(dir_before_chdir_backup);
        }
    } else if (!strcmp(commands[0], "cd") && !commands[1]) {
        getcwd(dir_before_chdir, PATH_MAX);
        chdir(commands[1]);
    } else if (!strcmp(commands[0], "cd") && commands[1]) {
        getcwd(dir_before_chdir, PATH_MAX);
        chdir(commands[1]);
        getcwd(dir_after_chdir, PATH_MAX);

        if (!strcmp(dir_before_chdir, dir_after_chdir)) {
            printf("No such directory\n");
        }
    }
    return;
}

void colors() {
    if (!strcmp(commands[0], "colors") && !commands[1]) {
        printf("\nTo change colors use command: 'color {name of color on list}'\n");
        printf("-------------------------------------------------------------\n");
        printf("List of available colors:\n1.black\n2.red\n3.green\n4.yellow\n5.blue\n6.purple\n7.cyan\n8.white\n9.reset\n");
        printf("-------------------------------------------------------------\n");
    } else if (!strcmp(commands[0], "colors") && commands[1]) {
        printf("Too many arguments for function 'colors'\n");
    }
    return;
}

void color() {
    if (!strcmp(commands[0], "color")) {
        if (!commands[1]) {
            printf("No color selected, please try again.\nFor help type 'colors'\n");
        } else if (!strcmp(commands[1], "black") && !commands[2]) {
            printf(BLACK);
        } else if (!strcmp(commands[1], "red") && !commands[2]) {
            printf(RED);
        } else if (!strcmp(commands[1], "green") && !commands[2]) {
            printf(GREEN);
        } else if (!strcmp(commands[1], "yellow") && !commands[2]) {
            printf(YELLOW);
        } else if (!strcmp(commands[1], "blue") && !commands[2]) {
            printf(BLUE);
        } else if (!strcmp(commands[1], "purple") && !commands[2]) {
            printf(PURPLE);
        } else if (!strcmp(commands[1], "cyan") && !commands[2]) {
            printf(CYAN);
        } else if (!strcmp(commands[1], "white") && !commands[2]) {
            printf(WHITE);
        } else if (!strcmp(commands[1], "reset") && !commands[2]) {
            printf(RESET);
        } else {
            printf("Could not match color, please try again.\nFor help type 'colors'\n");
        }
    }
    return;
}

void clear() {
    if (!strcmp(commands[0], "clear")) {
        printf("\e[H\e[2J\e[3J");
    }
    return;
}

void my_mv_files() {
    int destination_file = 0;
    int source_file = 0;
    int remove_file = 0;
    char buf[1024] = {0};
    int bytes_read = 0;
    if (!strcmp(commands[0], "mv") && !strcmp(commands[1], "-my") && (!commands[2] || !commands[3])) {
        printf("Mising file operand\n");
    } else if (!strcmp(commands[0], "mv") && !strcmp(commands[1], "-my") && !strcmp(commands[2], commands[3])) {
        printf("'%s' and '%s' are the same file\n", commands[2], commands[3]);
    } else if (!strcmp(commands[0], "mv") && !strcmp(commands[1], "-my") && commands[2] && commands[3] && commands[4]) {
        printf("Too much arguments to rename: %d\n", arguments_counter - 1);
    } else if (!strcmp(commands[0], "mv") && !strcmp(commands[1], "-my") && commands[2] && commands[3] && !commands[4]) {
        source_file = open(commands[2], O_RDONLY);
        if (source_file < 0) {
            printf("Cannot find file or permission to file is denied: %s\n", commands[2]);
        }
        destination_file = open(commands[3], O_CREAT | O_RDWR, 0644);
        while ((bytes_read = read(source_file, buf, sizeof(buf))) > 0) {
            if (write(destination_file, buf, bytes_read) != bytes_read) {
                printf("Error writing to destination file: %s\n", commands[3]);
                break;
            }
        }
        close(source_file);
        close(destination_file);
        remove_file = remove(commands[2]);
        if (remove_file == -1) {
            printf("Unable to remove source file\n");
        }
    }
}

void my_cp_files() {
    int cp_dest_file = 0;
    int cp_source_file = 0;
    char cp_buf[1024] = {0};
    int cp_bytes_read = 0;
    if (strcmp(commands[0], "cp") && !strcmp(commands[1], "-my")) {
        printf("Command not recognized.\nDid you mean 'mv -my' or 'cp -my'?\n");
    } else if (!strcmp(commands[0], "cp") && !strcmp(commands[1], "-my") && (!commands[2] && !commands[3])) {
        printf("Missing file operand\n");
    } else if (!strcmp(commands[0], "cp") && !strcmp(commands[1], "-my") && commands[2] && commands[3] && commands[4]) {
        printf("Too much arguments to copy file %d: \n", arguments_counter - 1);
    } else if (!strcmp(commands[0], "cp") && !strcmp(commands[1], "-my") && !strcmp(commands[2], commands[3])) {
        printf("'%s' and '%s' are the same file\n", commands[2], commands[3]);
    } else if (!strcmp(commands[0], "cp") && !strcmp(commands[1], "-my") && commands[2] && commands[3] && !commands[4]) {
        cp_dest_file = open(commands[3], O_CREAT | O_RDWR, 0644);
        cp_source_file = open(commands[2], O_RDONLY);
        if (cp_source_file < 0) {
            printf("Cannot find file or persmission to file is denied: %s\n", commands[2]);
        }
        while ((cp_bytes_read = read(cp_source_file, cp_buf, sizeof(cp_buf))) > 0) {
            if (write(cp_dest_file, cp_buf, cp_bytes_read) != cp_bytes_read) {
                printf("Error writing to destination file: %s\n", commands[3]);
                break;
            }
        }
        close(cp_source_file);
        close(cp_dest_file);
    }

}