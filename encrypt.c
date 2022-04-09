#include "encrypt.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

int main() {
    char* cmd = malloc(10);
    char* word = malloc(100);
    char* key = malloc(100);
    char* msg = malloc(200);
    char* tmp = malloc(200);
    char* result = "";
    char* error = "";
    int isErr = 0;
    do {
        read(STDIN_FILENO, tmp, 200);
        // reformat the read string
        sscanf(tmp, "%s %s", cmd, word);
        if(strcmp(cmd, "PASSKEY") == 0) {
            strcpy(key, word);
        } else if(strcmp(cmd, "ENCRYPT") == 0) {
            //encryption
            strcpy(msg, word);
            if(strlen(key) == 0) {
                error = "Password not set";
                isErr = 1;
            } else {
                result = encryption(msg, key);
            }
        } else if(strcmp(cmd, "DECRYPT") == 0) {
            //decryption
            strcpy(msg, word);
            if(strlen(key) == 0) {
                error = "Password not set";
                isErr = 1;
            } else {
                result = decryption(msg, key);
            }
        } else {
            //error
        }

        // send result back
        if(isErr) {
            strcpy(tmp, "ERROR ");
            strcat(tmp, error);
            strcat(tmp, "\n");
        } else {
            strcpy(tmp, "RESULT ");
            strcat(tmp, result);
            strcat(tmp, "\n");
        }
        write(STDOUT_FILENO, tmp, strlen(tmp));
        isErr = 0;
        result = "";
        error = "";

    } while(strcmp(cmd, "QUIT") != 0);

    return 0;
}

char* encryption(char* msg,  char* key) {
    char* ans = malloc(200);
    strcpy(ans, "");
    for(int i = 0, j = 0; i < strlen(msg); i++) {
        char ch = (char) toupper(msg[i]);
        if(ch < 'A' || ch > 'Z') {
            continue;
        }
        char s = (char)((ch + key[j]) % 26 + 'A');
        strncat(ans, &s, 1);
        j = ++j % strlen(key);
    }
    return ans;

}

char* decryption(char* msg,  char* key) {
    char* ans = malloc(200);
    strcpy(ans, "");
    for(int i = 0, j = 0; i < strlen(msg); i++) {
        char ch = (char) toupper(msg[i]);
        if(ch < 'A' || ch > 'Z') {
            continue;
        }
        char s = (char)((ch - key[j] + 26) % 26 + 'A');
        strncat(ans, &s, 1);
        j = ++j % strlen(key);
    }
    return ans;
}
