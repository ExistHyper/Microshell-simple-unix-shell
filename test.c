#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h> // tworzenie plikow...

#define BLACK "\033[0;30m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"
#define RESET "\033[0m"

void tokenization(char input_token[256]);
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
void my_mv();

char arr_of_commands[][32] = {
  "hostname",
  "whoami",
  "exit",
  "pwd",
  "help",
  "cd",
  "cd ~",
  "cd ..",
  "cd /",
  "clear",
  "bash",
  "colors",
  "color",
  "mv"
};
char *commands[256];
char input[256];
int arguments_counter = 0;

int main() {
  int i = 0;
  printf(RED "****WELCOME TO MICROSHELL****\n"
    RESET);
  printf("\n");

  while (1) {
    char current_dir[PATH_MAX];
    char prompt_hostname[_SC_HOST_NAME_MAX];
    gethostname(prompt_hostname, 40);
    getcwd(current_dir, PATH_MAX);
    printf(GREEN "%s@%s" RESET ":[%s] $ ", getlogin(), prompt_hostname, current_dir);
    fgets(input, 256, stdin);
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
    my_mv();
  }
  return 0;
}

void tokenization(char input_token[256]) {
  int i = 0;
  char *token_command = strtok(input_token, " \n\t");
  while (token_command != NULL) {
    commands[i++] = token_command;
    token_command = strtok(NULL, " \n\t");
  }
  arguments_counter = i - 1;
  commands[i++] = NULL;
  return;
}

void fork_and_unknown_commands() {
  for (int i = 0; i < 14; i++) {
    if (strcmp(arr_of_commands[i], commands[0]) == 0) {
      return;
    }
  }
  int execvp_value;
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
    gethostname(hostname, 40);
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
    printf("List of commands:\n'exit'- leave microshell\n'cd' - change directory\n'pwd' - display current directory\n'whoami' - display login\n'hostname' - display hostname\n'colors' - change display colors\n");
    printf("Autor Jan Kordas\nWydzial informatyki i informatyki UAM Poznan\n");
  }
  return;
}

void my_cd() {
  char dir_before_chdir[PATH_MAX];
  getcwd(dir_before_chdir, PATH_MAX);
  char dir_after_chdir[PATH_MAX];

  if (!strcmp(commands[0], "cd") && !commands[1]) {
    chdir(getenv("HOME"));
  } else if (!strcmp(commands[0], "cd") && !strcmp(commands[1], "~")) {
    chdir(getenv("HOME"));
  } else if (!strcmp(commands[0], "cd") && !strcmp(commands[1], "..")) {
    chdir("..");
  } else if (!strcmp(commands[0], "cd") && !strcmp(commands[1], "/")) {
    chdir("/");
  } else if (!strcmp(commands[0], "cd") && commands[1] && !commands[2]) {
    chdir(commands[1]);
    getcwd(dir_after_chdir, PATH_MAX);

    if (!strcmp(dir_before_chdir, dir_after_chdir)) {
      printf("No such directory\n");
    }
  } else if (!strcmp(commands[0], "cd") && commands[1] && commands[2]) {
    printf("Too many arguments\n");
  }
  return;
}

void colors() {
  if (!strcmp(commands[0], "colors") && !commands[1]) {
    printf("To change colors use command: 'color {name of color in list}'\n");
    printf("List of available colors:\n1.black\n2.red\n3.green\n4.yellow\n5.blue\n6.purple\n7.cyan\n8.white\n9.reset\n");
  } else if (!strcmp(commands[0], "colors") && commands[1]) {
    printf("Too many arguments for function 'colors'\n");
  }
  return;
}

void color() {
  if (!strcmp(commands[0], "color")) {
    if (commands[1] == NULL) {
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
    printf("\e[1;1H\e[2J");
  }
  return;
}

void my_mv() {
    if (!strcmp(commands[0],"mv") && (!commands[1] || !commands[2])) {
        printf("Missing file operand\n");
        return;
    } else if (!strcmp(commands[0],"mv") && commands[1] && commands[2]) {

        char *src = commands[1];
        char *dest = commands[2];

        int src_fd = open(src, O_RDONLY);
        if (src_fd < 0) {
            printf("Cannot find file or permission to file is denied: %s\n", src);
            return;
        }
        int dest_fd = open(dest, O_RDWR | O_CREAT, 0644);
        if (dest_fd < 0) {
            printf("Cannot open destination file: %s\n", dest);
            return;
        }
        char buf[1024];
        ssize_t bytes_read;
        while ((bytes_read = read(src_fd, buf, sizeof(buf))) > 0) {
            if (write(dest_fd, buf, bytes_read) != bytes_read) {
                printf("Error writing to destination file: %s\n", dest);
                break;
            }
        }
        close(src_fd);
        close(dest_fd);
        if (remove(src) < 0) {
            printf("Error removing source file: %s\n", src);
        }
    }
}
