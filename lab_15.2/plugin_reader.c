#include "plugin_reader.h"

void **lib_descr;
int lib_count;

int plugin_func_list_init(const char *dir_name,
                          struct plugin_func_struct **f_list)
{
    (*f_list) = NULL;
    struct dirent **lib_list;
    if (chdir(dir_name) != 0)
        return 1;
    lib_count = scandir(".", &lib_list, plugin_filter, alphasort);
    if (lib_count < 0) {
        perror("plugin_func_list_init->scandir error\n");
        return 1;
    }
    lib_descr = (void **) malloc(sizeof(void *) * lib_count);
    *f_list = NULL;
    struct plugin_func_struct *list_ptr = NULL;
    while (lib_count--) {
        char file_path[PATH_MAX] = "./";
        strcat(file_path, lib_list[lib_count]->d_name);
        lib_descr[lib_count] = dlopen(file_path, RTLD_LAZY);
        if (lib_descr == NULL) {
            perror("plugin_func_list_init->dlopen error\n");
            return 1;
        }
        int *count;
        char *name;
        *(void **)(&count) = dlsym(lib_descr[lib_count], "f_count");
        *(void **)(&name) = dlsym(lib_descr[lib_count], "f_name");
        if(count != NULL && name != NULL) {
            for(int it = 0; it < *count; ++it, name += MAX_F_NAME_LENGTH) {
                list_ptr = (struct plugin_func_struct *)
                           malloc(sizeof(struct plugin_func_struct));
                *(void **)(&(list_ptr->f_ptr)) = dlsym(lib_descr[lib_count],
                                                       name);
                list_ptr->f_name = calloc(MAX_F_NAME_LENGTH, sizeof(char));
                strcpy(list_ptr->f_name, name);
                list_ptr->next = (*f_list);
                (*f_list) = list_ptr;
            }
        }
        free(lib_list[lib_count]);
    }
    free(lib_list);
    return 0;
}


int plugin_filter(const struct dirent *ent)
{
    if (fnmatch("*.so", ent->d_name, 0) == 0)
        return 1;
    return 0;
}


void plugin_func_list_free(struct plugin_func_struct **f_list)
{
    struct plugin_func_struct *list_ptr = (*f_list);
    struct plugin_func_struct *rm_struct_ptr;
    for(int it = 0; it < lib_count; ++it) {
        dlclose(lib_descr[it]);
        free(lib_descr[it]);
    }
    free(lib_descr);
    while (list_ptr != NULL) {
        rm_struct_ptr = list_ptr;
        free(rm_struct_ptr->f_name);
        list_ptr = rm_struct_ptr->next;
        free(rm_struct_ptr);
    }
}


struct plugin_func_struct *get_plugin_f(struct plugin_func_struct *f_list,
                                        const unsigned int number)
{
    struct plugin_func_struct *f_list_it = f_list;
    for(unsigned int it = 1; it < number && f_list_it != NULL;
        ++it, f_list_it = f_list_it->next) continue;
    if(f_list_it != NULL)
        return f_list_it;
    return NULL;
}
