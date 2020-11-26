/* NRSS - ncurses RSS reader
   Copyright (C) 2007 Jack Miller <jjm2n4@umr.edu>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 as 
   published by the Free Software Foundation.
*/
#include "nrss.h"

char *read_string_from_fd(int source)
{
    char *str = NULL;
    char *tmp = NULL;
    char *buff = xmalloc(BUFFER + 1);
    int len = 0;

    while ((len = read(source, buff, BUFFER)) > 0) {
        buff[len] = 0;

        if (str) {
            tmp = concat(str, buff);
            free(str);
            str = tmp;
        } else
            str = strdup(buff);
    }

    free(buff);
    return str;
}


void write_string_to_fd(const char *str, int dest)
{
    size_t len = 0;
    size_t index = 0;
    size_t bytes = 0;

    if (!str)
        return;

    len = strlen(str);
    while (index < len) {
        bytes = write(dest, &(str[index]), len - index);
        index += bytes;
    }
}

char *filter_description(char *str)
{
    char *filtered_string = NULL;

    if (!nc->desc_filter_line)
        return NULL;

    filtered_string = filter_string(str, nc->desc_filter_line);
    return filtered_string;
}
