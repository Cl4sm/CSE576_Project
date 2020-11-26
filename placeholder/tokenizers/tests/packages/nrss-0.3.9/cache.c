/* NRSS - ncurses RSS reader
   Copyright (C) 2007 Jack Miller <jjm2n4@umr.edu>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 as 
   published by the Free Software Foundation.
*/
#include "nrss.h"

char *get_cache_descr(int start, int len)
{
    FILE *fp = NULL;
    char *line = NULL;

    if ((fp = fopen(nc->cache_file, "r")) != NULL) {
        line = xmalloc(len + 1);
        fseek(fp, start, SEEK_SET);
        fread(line, len, 1, fp);
        line[len] = 0;
        fclose(fp);
    }

    return line;
}

void desc_cache(char *descr, int *start, int *len)
{
    FILE *fp = NULL;

    if(!descr)
        return;

    if ((fp = fopen(nc->cache_file, "a")) != NULL) {
        *start = ftell(fp);
        *len = strlen(descr);
        fprintf(fp, "%s", descr);
        fclose(fp);
    }
}
