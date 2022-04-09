
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "logger.h"

int main() {
    char* tmp = malloc(60);
    char filename[20];
    // read the file name
    read(STDIN_FILENO, tmp, 20);
    sscanf(tmp, "%[^\n]s", filename);
    FILE* logFile = fopen(filename, "w+a");
    //respond driver program with successful open file
    write(STDOUT_FILENO, "Log file opened.\n", 17);

    char action[10];
    char msg[50];
    char* logLine = NULL;

    while(1) {
        // get the next log entry
        read(STDIN_FILENO, tmp, 200);
        // reformat the string
        sscanf(tmp, "%s %[^\n]s", action, msg);
        logLine = getLogLine(action, msg);
        fprintf(logFile, "%s\n", logLine);
        if(strcmp(action, "QUIT") == 0) {
            break;
        }
        strcpy(action, "");
        strcpy(msg, "");
    }
    fclose(logFile);
    return 0;
}

char* getTimeString() {
    char* buff = malloc(50);
    time_t now = time(NULL);
    strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
    return buff;
}

char* getLogLine(char* action, char* msg) {
    char* res = getTimeString();
    strcat(res, " ");
    strcat(res, action);
    strcat(res, " ");
    strcat(res, msg);
    return res;
}

