/* NRSS - ncurses RSS reader
   Copyright (C) 2007 Jack Miller <jjm2n4@umr.edu>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 as 
   published by the Free Software Foundation.
*/
#include "nrss.h"

void logit(char *format, ...)
{
    FILE *fp = NULL;
    va_list ap;

    if ((fp = fopen(nc->log_file, "a")) > 0) {
        va_start(ap, format);
        vfprintf(fp, format, ap);
        va_end(ap);
        fclose(fp);
    }
}
