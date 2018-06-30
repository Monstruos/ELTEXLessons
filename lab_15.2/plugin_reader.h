#ifndef __PLUGIN_READER__H
#define __PLUGIN_READER__H
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <fnmatch.h>
#include <string.h>
#include <limits.h>

#define MAX_F_NAME_LENGTH 256

struct plugin_func_struct {
    float (*f_ptr) (int, int);
    char *f_name;
    struct plugin_func_struct *next;
};


int plugin_func_list_init(const char *, struct plugin_func_struct **);
int plugin_filter(const struct dirent *);
void plugin_func_list_free(struct plugin_func_struct **);
struct plugin_func_struct *get_plugin_f(struct plugin_func_struct *f_list,
                                        const unsigned int number);

#endif // __PLUGIN_READER__H
