//
// Created by Inna Zhurba on 16.10.2022.
//

#ifndef MYSHELL_INTERNAL_CMD_H
#define MYSHELL_INTERNAL_CMD_H

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include "options_parser.h"

int mecho (char** argv);
int mexport (char** argv);
int mcd (char** argv);
int mrun (char** argv);
int dot_run (char** argv);

#endif //MYSHELL_INTERNAL_CMD_H
