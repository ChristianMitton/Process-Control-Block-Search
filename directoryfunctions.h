#ifndef DIRECTORYFUNCTIONS_H
#define DIRECTORYFUNCTIONS_H

#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

void listdir(const char *name, int indent);

#endif
