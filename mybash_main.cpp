// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>

#include "options_parser.h"
#include "include/internal_cmd.h"

#include <readline/readline.h>
#include <readline/history.h>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::strlen;
using std::strcat;

extern char **environ;

//implementation of the function get_current_dir_name() for Linux
char *get_current_dir_name() {
    char *buf = getcwd(nullptr, 0);
    if (buf == nullptr) {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }
    return buf;
}

int main() {
    int merrno = 0;

    char* command;
    while ((command = readline(strcat(get_current_dir_name (), "$ "))) != nullptr) {
        if (strlen(command) > 0) {
          add_history(command);
        }

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

        char** argv = new char*[args.size() + 1];
        size_t argc = args.size();
        for ( size_t k = 0; k < args.size(); k++ )
            argv[k] = args[k];

        argv[args.size()] = NULL;

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
                pid_t parent = getpid();
                pid_t pid = fork();

                if(pid == -1) {
                    cerr << "Failed to fork()" << endl;
                    exit(EXIT_FAILURE);
                } else if (pid > 0) {
                    cout << "Parent: Hello from parent" << endl;
                    cout << "Parent: Parent  PID: " << parent << ", child PID: " << pid << endl;
                    int status;
                    waitpid(pid, &status, 0);
                    cout << "Parent: child stopped, exit code: " << status << endl;
                } else {
                    string child_name = argv[0];

                    vector<string> args;
                    args.push_back(child_name);

                    for(size_t a = 1; a < argc; ++a) {
                        cout << argv[a] << endl;
                        args.push_back(argv[a]);
                    }

                    vector<const char*> arg_for_c;

                    for(const auto& s: args)
                        arg_for_c.push_back(s.c_str());
                    arg_for_c.push_back(nullptr);

                    execvp(child_name.c_str(), const_cast<char* const*>(arg_for_c.data()));

                    cerr << "Parent: Failed to execute " << child_name << "\n\tCode: " << errno << endl;
                    exit(EXIT_FAILURE);
                }
            }
        }

        free(command);
    }

    return 0;
}
