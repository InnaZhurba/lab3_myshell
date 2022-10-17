// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <glob.h>
#include "options_parser.h"
#include "include/internal_cmd.h"

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
    //errno
    int merrno = 0;

    while (true) {
        // Show prompt.
        std::cout << get_current_dir_name () << "$ " ;
        char command[128];
        std::cin.getline( command, 128 );

        //ignore everything after # in command
        char* comment = strchr(command, '#');
        if (comment != nullptr) {
            *comment = '\0';
        }

        std::vector<char*> args;
        char* prog = strtok( command, " " );
        char* tmp = prog;
        while ( tmp != NULL ) {
            args.push_back( tmp );
            tmp = strtok( NULL, " " );
        }
        //showing the list of arguments
        std::cout << "ARGS" << std::endl;
        for (int i = 0; i < args.size(); ++i) {
            std::cout << args[i] << std::endl;
        }

        //wildcard_search(args);

        char** argv = new char*[args.size()+1];
        for ( int k = 0; k < args.size(); k++ )
            argv[k] = args[k];

        argv[args.size()] = NULL;

        //show the list of arguments
        std::cout << "ARGV" << std::endl;
        for (int i = 0; i < args.size(); ++i) {
            std::cout << argv[i] << std::endl;
        }

        if ( strcmp( command, "mexit" ) == 0 )
        {
            //return exit code from command
            if (argv[1]==NULL)
                return 0;
            return atoi(argv[1]);
        }
        else
        {
            if ( !strcmp (prog, "merrno") ){
                std::cout << merrno << std::endl;
                perror (command);
            }
            else if ( !strcmp (prog, "mpwd") ){
                std::cout << get_current_dir_name() << std::endl;
                perror (command);
            }
            else if ( !strcmp (prog, "mecho") ) {
                merrno = mecho(argv);
            }
            else if ( !strcmp (prog, "mexport") ) {
                merrno = mexport(argv);
            }
            else if ( !strcmp (prog, "mcd")) {
                merrno = mcd(argv);
            }
                //run smth with command run
            else if ( !strcmp (prog, "mrun") ) {
                merrno = mrun(argv);
            }
                //run .msh file script
            else if ( !strcmp (prog, ".") ) {
                merrno = dot_run(argv);
            }
            else if ( !strcmp (prog, "mhelp") ) {
                // print help
                // implement it
                std::cout << "mexit - exit from shell" << std::endl;
                std::cout << "merrno - print last error code" << std::endl;
                std::cout << "mpwd - print current directory" << std::endl;
                std::cout << "mecho - print string" << std::endl;
                std::cout << "mexport - export variable" << std::endl;
                std::cout << "mcd - change directory" << std::endl;
                std::cout << "mrun - run script" << std::endl;
                std::cout << ".<name> - print history of commands" << std::endl;
                std::cout << "mhelp - print help" << std::endl;
                merrno = 0;
            }
            else {
                pid_t kidpid = fork();

                if (kidpid < 0) {
                    perror( "Internal error: cannot fork." );
                    return -1;
                }
                else if (kidpid == 0) {
                    // the child.
                    execvp (prog, argv);

                    // usually don`t happend
                    perror( command );
                    return -1;
                }
                else {
                    waitpid( kidpid, &merrno, 0 );
                    // the parent.  is waiting for the child.
                    if ( merrno < 0 ) {
                        perror( "Internal error: cannot wait for child." );
                        return -1;
                    }
                }
            }
        }
    }
}

