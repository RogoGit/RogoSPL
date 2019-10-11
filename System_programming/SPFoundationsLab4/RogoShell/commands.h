//
// Created by dell on 11.10.19.
//

#ifndef ROGOSHELL_COMMANDS_H
#define ROGOSHELL_COMMANDS_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<time.h>
#include<readline/readline.h>
#include<readline/history.h>

int take_input(char* str);

//where the system command is executed
void exec_comm(char** parsed);

// where the piped system commands is executed
void exec_pipe(char** parsed, char** parsedpipe);

#endif //ROGOSHELL_COMMANDS_H
