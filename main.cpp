// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include "options_parser.h"

std::string chararr_to_string(char *argv[]) {
    std::string str;
    for (int i = 0; argv[i] != nullptr; ++i) {
        str += argv[i];
        str += " ";
    }
    return str;
}

//implementation of the function get_current_dir_name() for Linux
char *get_current_dir_name() {
    char *buf = getcwd(nullptr, 0);
    if (buf == nullptr) {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }
    return buf;
}


// implement fork and exec
int main() {//(int argc, char *argv[], char* envp[]) {
    // command_line_options_t command_line_options{argc, argv};
    // std::cout << "A flag value: " << command_line_options.get_A_flag() << std::endl;

    //errno
    int merrno = 0;

    //saving history of the commands in vector
    //!!!!!! save only if there are no errors
    std::vector<std::string> history;
    //history.push_back(chararr_to_string(argv));

    while (true) {
        // Show prompt.
        std::cout << get_current_dir_name () << "$ " ;
        char command[128];
        std::cin.getline( command, 128 );

        // saving history of the commands in vector
        history.push_back(command);

        // splitting string into tokens
        std::vector<char*> args;
        char* prog = strtok( command, " " );
        char* tmp = prog;
        while ( tmp != NULL ) {
            args.push_back( tmp );
            tmp = strtok( NULL, " " );
        }

        char** argv = new char*[args.size()+1];
        for ( int k = 0; k < args.size(); k++ )
            argv[k] = args[k];

        argv[args.size()] = NULL;

        if ( strcmp( command, "exit" ) == 0 )
        {
            return 0;
        }
        else
        {
            if (!strcmp (prog, "cd"))
            {
                if (argv[1] == NULL)
                {
                    chdir ("/");
                }
                else
                {
                    chdir (argv[1]);
                }
                perror (command);
            }
            if ( !strcmp (prog, "merrno") ){
                std::cout << merrno << std::endl;
                perror (command);
            }
            if ( !strcmp (prog, "mpwd") ){
                std::cout << get_current_dir_name() << std::endl;
                perror (command);
            }

            else
            {
                pid_t kidpid = fork();

                if (kidpid < 0)
                {
                    perror( "Internal error: cannot fork." );
                    return -1;
                }
                else if (kidpid == 0)
                {
                    // the child.
                    execvp (prog, argv);

                    // usually don`t happend
                    perror( command );
                    return -1;
                }
                else
                {
                    waitpid( kidpid, &merrno, 0 );
                    // the parent.  is waiting for the child.
                    if ( merrno < 0 )
                    {
                        perror( "Internal error: cannot wait for child." );
                        return -1;
                    }
                }
            }
        }
    }

    return 0;
}

