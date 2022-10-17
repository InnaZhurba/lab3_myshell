//
// Created by Inna Zhurba on 10.10.2022.
//

#include "../include/internal_cmd.h"

void wildcard_search(std::vector<char*> &args) {
    // implement search by wildcards (glob) and replace the command with the list of files that match the pattern (glob)
    // (if there are no files that match the pattern, the command is not executed)

    // old version
    /*glob_t globbuf;
    glob(command, 0, NULL, &globbuf);
    for (int i = 0; i < globbuf.gl_pathc; i++) {
        std::cout << globbuf.gl_pathv[i] << std::endl;
    }
    globfree(&globbuf);
    */

    // new version
    glob_t globbuf;
    globbuf.gl_offs = 1;
    glob(args[0], GLOB_DOOFFS, NULL, &globbuf);
    args.clear();
    for (int i = 0; i < globbuf.gl_pathc; ++i) {
        args.push_back(globbuf.gl_pathv[i]);
    }
}

int mecho (char** argv) {
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
    return 0;
}
int mexport (char** argv) {
    // if there is no variable with such name then we create it
    // if there is variable with such name then we change its value
    // implement it
    if (argv[1] == NULL)
    {
        return 1;
    }
    else
    {
        std::string str = argv[1];
        std::string name = str.substr(0, str.find('='));
        std::string value = str.substr(str.find('=') + 1);
        setenv(name.c_str(), value.c_str(), 1);
        return 1;
    }
}
int mcd (char** argv) {
    //implementation of cd command
    //know about ~ and ..
    if (argv[1] == NULL) {
        return 1;
    }
    else {
        std::string str = argv[1];
        if (str[0] == '~') {
            str = getenv("HOME") + str.substr(1);
        }
        if (chdir(str.c_str()) == -1) {
            return 1;
        } else {
            return 0;
        }
    }
}
int mrun (char** argv) {
    // if there is no script with such name then we print error
    // if there is script with such name then we run it
    // implement it
    if (argv[1] == NULL)
    {
        return 1;
    }
    else
    {
        std::string str = argv[1];
        if (str[0] == '~') {
            str = getenv("HOME") + str.substr(1);
        }
        if (chdir(str.c_str()) == -1) {
            return 1;
        } else {
            return 0;
        }
    }
}
int dot_run (char** argv) {
    // if there is no script with such name then we print error
    // if there is script with such name then we run it
    // implement it
    if (argv[1] == NULL)
    {
       return 1;
    }
    else
    {
        std::string str = argv[1];
        if (str[0] == '~') {
            str = getenv("HOME") + str.substr(1);
        }
        if (chdir(str.c_str()) == -1) {
            return 1;
        } else {
            return 0;
        }
    }
}