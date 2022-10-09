// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "options_parser.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::strlen;
using std::strcat;

extern char **environ;

int main(int argc, char* argv[]) {
    command_line_options_t command_line_options{argc, argv};

    if (argc < 2) {
        return -1;
    }

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
        string child_name = argv[1];

        vector<string> args;
        args.push_back(child_name);
        // run as follows: ./mybash ./mycat ...

        for(int a = 2; a < argc; ++a)
            args.push_back(argv[a]);
//        args.push_back("readme.md");

        vector<const char*> arg_for_c;

        for(const auto& s: args)
            arg_for_c.push_back(s.c_str());
        arg_for_c.push_back(nullptr);

        execvp(child_name.c_str(), const_cast<char* const*>(arg_for_c.data()));

        cerr << "Parent: Failed to execute " << child_name << "\n\tCode: " << errno << endl;
        exit(EXIT_FAILURE);
    }

    return 0;
}
