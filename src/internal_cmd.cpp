//
// Created by Inna Zhurba on 10.10.2022.
//

#include "../include/internal_cmd.h"

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