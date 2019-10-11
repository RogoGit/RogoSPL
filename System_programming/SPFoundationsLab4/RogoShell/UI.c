//
// Created by dell on 11.10.19.
//

#include "UI.h"

// print current info
void print_curr_info() {

    char cwd[1024];
    char* username = getenv("USER");
    time_t now;
    time(&now);
    printf("\nHello, %s", username);
    printf("\nIt is : %s", ctime(&now));
    getcwd(cwd, sizeof(cwd));
    printf("And you are in: %s", cwd);

}