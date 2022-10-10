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

        if ( strcmp( command, "mexit" ) == 0 )
        {
            return 0;
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
                // if there is $ in the string then we need to replace it with the value of the variable
                // if there is no $ then we just print the string
                // if there is no variable with such name then we print nothing
                // implement it
                if (argv[1] == NULL)
                {
                    std::cout << std::endl;
                }
                else
                {
                    int num = 1;
                    while (argv[num]!=NULL) {
                        std::string str = argv[num];
                        if (str[0] == '$') {
                            char *env = getenv(str.substr(1).c_str());
                            if (env != NULL) {
                                std::cout << env << " ";
                            }
                        } else {
                            std::cout << str << " ";
                        }
                        num++;
                    }

                    std::cout << std::endl;
                }
                merrno = 0;
            }
            else if ( !strcmp (prog, "mexport") ) {
                // if there is no variable with such name then we create it
                // if there is variable with such name then we change its value
                // implement it
                if (argv[1] == NULL)
                {
                    merrno = 1;
                }
                else
                {
                    std::string str = argv[1];
                    std::string name = str.substr(0, str.find('='));
                    std::string value = str.substr(str.find('=') + 1);
                    setenv(name.c_str(), value.c_str(), 1);
                    merrno = 0;
                }
            }
            else if ( !strcmp (prog, "mcd")) {
                //implementation of cd command
                //know about ~ and ..
                if (argv[1] == NULL) {
                    merrno = 1;
                }
                else {
                    std::string str = argv[1];
                    if (str[0] == '~') {
                        str = getenv("HOME") + str.substr(1);
                    }
                    if (chdir(str.c_str()) == -1) {
                        merrno = 1;
                    } else {
                        merrno = 0;
                    }
                }
            }
                //run smth with command run
            else if ( !strcmp (prog, "mrun") ) {
                // if there is no script with such name then we print error
                // if there is script with such name then we run it
                // implement it
                if (argv[1] == NULL)
                {
                    merrno = 1;
                }
                else
                {
                    std::string str = argv[1];
                    if (str[0] == '~') {
                        str = getenv("HOME") + str.substr(1);
                    }
                    if (chdir(str.c_str()) == -1) {
                        merrno = 1;
                    } else {
                        merrno = 0;
                    }
                }
            }
                //run .msh file script
            else if ( !strcmp (prog, ".") ) {
                // if there is no script with such name then we print error
                // if there is script with such name then we run it
                // implement it
                if (argv[1] == NULL)
                {
                    merrno = 1;
                }
                else
                {
                    std::string str = argv[1];
                    if (str[0] == '~') {
                        str = getenv("HOME") + str.substr(1);
                    }
                    if (chdir(str.c_str()) == -1) {
                        merrno = 1;
                    } else {
                        merrno = 0;
                    }
                }
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

