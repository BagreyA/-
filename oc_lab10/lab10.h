#ifndef LAB10_H
#define LAB10_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define AUTHOR "Автор: Багрей А.О."

// Функции
void print_env_var_method1(char **env);
void print_env_var_method2();
void print_file_content(const char *filename);
void print_usage(const char *progname);

#endif // MYPROG_H
