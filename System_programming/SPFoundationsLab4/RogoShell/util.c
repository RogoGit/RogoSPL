//
// Created by dell on 11.10.19.
//

#include "util.h"
#include "macro.h"

void parse_by_space(char* str, char** parsed) {
    for (int i = 0; i < MAXCOMM; i++) {
        parsed[i] = strsep(&str, " ");
        if (parsed[i] == NULL) break;
        if (strlen(parsed[i]) == 0) i--;
    }
}

int parse_pipe(char* str, char** strpiped) {
    int i;
    for (i = 0; i < 2; i++) {
        strpiped[i] = strsep(&str, "|");
        if (strpiped[i] == NULL)
            break;
    }
    if (strpiped[1] == NULL)
        return 0; // zero if no pipe found.
    else {
        return 1;
    }
}

int handle_string(char* str, char** parsed, char** parsedpipe) {
    char* strpiped[2];
    int piped = 0;
    piped = parse_pipe(str, strpiped);
    if (piped) {
        parse_by_space(strpiped[0], parsed);
        parse_by_space(strpiped[1], parsedpipe);
    } else {
        parse_by_space(str, parsed);
    }
    // exit
    if (strcmp(parsed[0], "exit") == 0) {
        printf("\nBye. Have a good day\n");
        return 3;
    }
    // change directory
    if (strcmp(parsed[0], "cd") == 0) {
        chdir(parsed[1]);
        return 0;
    } else {
        return 1 + piped;
    }
}



