// RogoShell
#include "commands.h"
#include "UI.h"
#include "macro.h"
#include "util.h"

int main() {

    char input_string[MAXSYMB];
    char *parsed_args[MAXCOMM];
    char* parsed_args_pipe[MAXCOMM];
    int exec_mode = 0; // normal command or pipe or exit

    printf("\n|||||||||||||||||||||||||||||||||||||||||||||||||||||||");
    printf("\n///////////////////////////////////////////////////////\n");
    printf("-------------------------ROGOSHELL---------------------");
    printf("\n///////////////////////////////////////////////////////\n");
    printf("|||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");

    while (1) {
        // user interface line
        print_curr_info();
        // take input string
        if (take_input(input_string))
            continue;
        // handle the string
        exec_mode = handle_string(input_string, parsed_args, parsed_args_pipe);
        // decide how to execute
        if (exec_mode == 1)
            exec_comm(parsed_args);
        // pipe
        if (exec_mode == 2)
            exec_pipe(parsed_args, parsed_args_pipe);
        // exit
        if (exec_mode == 3)
            exit(0);
    }
}