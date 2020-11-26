/* NRSS - ncurses RSS reader
   Copyright (C) 2007 Jack Miller <jjm2n4@umr.edu>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 as 
   published by the Free Software Foundation.
*/
#include "nrss.h"
#include "conf.h"

int set_columns(void *data)
{
    if ((unsigned long) data < 0)
        return 1;

    if (columns_set())
        return 0;

    nc->defs[DEF_COLS] = ((int *) data)[0];
    return 0;
}

int check_theme(void *data)
{
    if (((unsigned long) data < 0) || (((unsigned long) data) > 7)) {
        return 1;
    }

    return 0;
}

int set_rate(void *data)
{
    struct feed *cur = get_feed(((char **) data)[1]);
    int num = ((int *) data)[0];

    if (cur) {
        if (num <= 0)
            cur->rate = 0;
        else
            cur->rate = num;

        return 0;
    }

    return 1;
}

int set_max(void *data)
{
    struct feed *cur = get_feed(((char **) data)[1]);
    int num = ((int *) data)[0];
    if (cur) {
        if (num <= 0)
            cur->keep = 0;
        else
            cur->keep = num;

        return 0;
    }

    return 1;
}

int set_show(void *data)
{
    struct feed *cur = get_feed(((char **) data)[1]);
    int num = ((int *) data)[0];

    if (cur) {
        if (num <= 0)
            cur->show = 0;
        else
            cur->show = num;

        return 0;
    }

    return 1;
}

int set_def(int def, int num)
{
    if (num < 0)
        nc->defs[def] = 0;
    else
        nc->defs[def] = num;

    return 0;
}

int set_def_max(void *data)
{
    return set_def(DEF_MAX, ((int *) data)[0]);
}

int set_def_rate(void *data)
{
    return set_def(DEF_RATE, ((int *) data)[0]);
}

int set_def_show(void *data)
{
    return set_def(DEF_SHOW, ((int *) data)[0]);
}

int set_theme(void *data)
{
    if ((((int *) data)[0] >= 1) && (((int *) data)[0] <= 8)) {
        nc->themes[((int *) data)[0] - 1].fg = ((int *) data)[1];
        nc->themes[((int *) data)[0] - 1].bg = ((int *) data)[2];
    }
    return 0;
}

int set_browser_wait(void *data)
{
    if (((int *) data)[0])
        nc->bits |= CFG_BITS_BROWSER_WAIT;
    else
        nc->bits &= ~(CFG_BITS_BROWSER_WAIT);

    return 0;
}

void escape_filter(char *line)
{
    char *sub = line;

    while ((sub = strstr(sub, "\\n")) != NULL) {
        if (sub != line) {
            if (sub[-1] != '\\')
                sub[0] = '\n';
            else
                sub--;
            memmove(&sub[1], &sub[2], strlen(sub) - 1);
        } else {
            sub[0] = '\n';
            memmove(&sub[1], &sub[2], strlen(sub) - 1);
        }

        sub++;
    }
}

int set_browser(void *data)
{
    nc->browser_line = ((char **) data)[0];
    return 0;
}

int set_wget(void *data)
{
    nc->wget_line = ((char **) data)[0];
    return 0;
}

int set_desc_filter(void *data)
{
    nc->desc_filter_line = ((char **) data)[0];
    return 0;
}

int set_story_filter(void *data)
{
    nc->story_filter_line = ((char **) data)[0];
    return 0;
}

int set_title_escape(void *data)
{
    escape_filter(((char **) data)[0]);
    nc->title_string = ((char **) data)[0];
    return 0;
}

int set_story_escape(void *data)
{
    escape_filter(((char **) data)[0]);
    nc->story_string = ((char **) data)[0];
    return 0;
}

int set_r_story_escape(void *data)
{
    escape_filter(((char **) data)[0]);
    nc->r_story_string = ((char **) data)[0];
    return 0;
}

int set_u_story_escape(void *data)
{
    escape_filter(((char **) data)[0]);
    nc->u_story_string = ((char **) data)[0];
    return 0;
}

int set_reader_escape(void *data)
{
    escape_filter(((char **) data)[0]);
    nc->descr_string = ((char **) data)[0];
    return 0;
}
