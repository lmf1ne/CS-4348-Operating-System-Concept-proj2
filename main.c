#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

int main() {
    pid_t pids[2]; // 0 is the encryption, 1 is the logger
    int pipe1[2], pipe2[2], pipe3[2], pipe4[2];  // pipe1 is the encryption input, pipe2 is the encryption output, pipe3 and pipe4 are the logger

    if(pipe(pipe1) < 0 || pipe(pipe2) < 0 || pipe(pipe3) < 0 || pipe(pipe4) < 0) {
        exit(0);
    }
    //give birth to logger process
    switch(pids[0] = fork()) {
        case -1:
            //error
            _exit(1);
        case 0:
            //this is the child process
            dup2(pipe3[0], STDIN_FILENO); //writing to pipe3 goes to stdin
            dup2(pipe4[1], STDOUT_FILENO); //writing to pipe3 goes to stdin
            close(pipe3[0]);
            close(pipe3[1]);
            close(pipe4[0]);
            close(pipe4[1]);
            // logger
            execlp("./logger", "logger", (char *) 0);
            _exit(3);
        default:
            //parent
            //give birth to the encrypt process
            switch(pids[1] = fork()) {
                case -1:
                    //error
                    break;
                case 0:
                    //this is the child process
                    dup2(pipe1[0], STDIN_FILENO); //writing to pipe1 goes to stdin
                    dup2(pipe2[1], STDOUT_FILENO); //stdout can be read from pipe2
                    close(pipe1[0]);
                    close(pipe1[1]);
                    close(pipe1[0]);
                    close(pipe2[1]);
                    // encrypt
                    execlp("./encrypt", "encrypt", (char *) 0);
                    _exit(3);
            }
    }

    /* parent */
    char* output = malloc(100);
    char* res = malloc(200);
    char* type = malloc(10);
    char* result = NULL;
    char filename[20];
    //get the name of the log file
    printf("Enter the log filename: ");
    scanf("%s", filename);

    write(pipe3[1], filename, strlen(filename));
    read(pipe4[0], filename, 20);

    strcpy(output, "START Logging Started.\n");
    write(pipe3[1], output, strlen(output));

    // prompt
    char* history[100];
    int size = 0;
    int choice = -1;
    char* curStr = NULL;
    char* line = malloc(200);
    const char* menu = "Enter the command\n1. password\n2. encrypt\n3. decrypt\n4. history\n5. quit\n";
    do {
        printf("%s", menu);
        scanf("%d", &choice);
        if(choice == 1 || choice == 2 || choice == 3) {
            // show history
            curStr = getNextString(history, size);
        }
        switch(choice) {
            case 1:
                strcpy(line, "PASSKEY ");
                strcat(line, curStr);
                write(pipe1[1], line, strlen(line));
                output = malloc(200);
                strcpy(output, line);
                strcat(output, "\n");
                write(pipe3[1], output, strlen(output));
                // read result
                read(pipe2[0], res, 200);
                break;
            case 2:
                strcpy(line, "ENCRYPT ");
                strcat(line, curStr);
                write(pipe1[1], line, strlen(line));
                output = malloc(200);
                strcpy(output, line);
                strcat(output, "\n");
                write(pipe3[1], output, strlen(output));
                // read result
                read(pipe2[0], res, 200);
                result = malloc(200);
                strcpy(result, "");
                sscanf(res, "%s %[^\n]s", type, result);
                if(strlen(result) > 0) {
                    history[size++] = result;
                }
                strcpy(output, "RESULT ");
                strcat(output, result);
                strcat(output, "\n");
                write(pipe3[1], output, strlen(output));
                break;
            case 3:
                strcpy(line, "DECRYPT ");
                strcat(line, curStr);
                write(pipe1[1], line, strlen(line));
                output = malloc(200);
                strcpy(output, line);
                strcat(output, "\n");
                write(pipe3[1], output, strlen(output));
                // read result
                read(pipe2[0], res, 200);
                result = malloc(200);
                strcpy(result, "");
                sscanf(res, "%s %[^\n]s", type, result);
                if(strlen(result) > 0) {
                    history[size++] = result;
                }
                strcpy(output, "RESULT ");
                strcat(output, result);
                strcat(output, "\n");
                write(pipe3[1], output, strlen(output));
                break;
            case 4:
                // print all history
                printAllHistory(history, size);
        }
    } while(choice != 5);

    // quit log out
    output = "QUIT STOP Logging Stopped.\n";
    // stop the child processes using QUIT
    write(pipe3[1], output, strlen(output));
    write(pipe1[1], output, strlen(output));
    // avoid zombie child processes
    for(int i = 0; i < 2; i++) {
        waitpid(-1, NULL, 0);
    }

    return 0;
}

void printAllHistory(char* h[], int size) {
    for(int i = 0; i < size; i++)
    {
        printf("%d. %s", i + 1, h[i]);
    }
}

char* getHistoryString(char* h[], int size) {
    printAllHistory(h, size);
    int loc = -1;
    scanf("%d", &loc);

    return h[loc - 1];
}

char* getNextString(char* h[], int size) {
    int select = -1;
    char* str = malloc(100);
    printf("Enter 1 for a new string or enter 2 for string in the history: ");
    scanf("%d", &select);
    if(size == 0) select = 1;
    switch(select) {
        case 1:
            //new string
            printf("Enter the new string: ");
            scanf("%s", str);
            break;
        case 2:
            str = getHistoryString(h, size);
            break;

        default:
            break;
    }
    return str;
}

