/* NRSS - ncurses RSS reader
   Copyright (C) 2007 Jack Miller <jjm2n4@umr.edu>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 as 
   published by the Free Software Foundation.
*/
#include "nrss.h"

int add_feed(void *data)
{
    struct feed *newfeed = NULL;

    newfeed = xmalloc(sizeof(struct feed));
    memset(newfeed, 0, sizeof(struct feed));

    newfeed->URL = ((char **) data)[0];
    newfeed->handle = ((char **) data)[1];
    newfeed->rate = nc->defs[DEF_RATE];
    newfeed->show = nc->defs[DEF_SHOW];
    newfeed->keep = nc->defs[DEF_MAX];
    newfeed->time = 1;

    ladd_feed(&nc->feeds, newfeed);

    return 0;
}

struct feed *get_feed(char *handle)
{
    foreach_feed(curfeed) {
        if (!strcmp(curfeed->handle, handle))
            return curfeed;
    }

    return NULL;
}
