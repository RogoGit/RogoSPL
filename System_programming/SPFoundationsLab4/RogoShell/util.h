//
// Created by dell on 11.10.19.
//

#ifndef ROGOSHELL_UTIL_H
#define ROGOSHELL_UTIL_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

// parsing command words
void parse_by_space(char* str, char** parsed);

// finding pipe
int parse_pipe(char* str, char** strpiped);

//
int handle_string(char* str, char** parsed, char** parsedpipe);

#endif //ROGOSHELL_UTIL_H
