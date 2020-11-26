/* NRSS - ncurses RSS reader
   Copyright (C) 2007 Jack Miller <jjm2n4@umr.edu>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 as 
   published by the Free Software Foundation.
*/
#include "nrss.h"
#include "interface.h"

#define NUM_READER_ESCAPES 3
static struct escape reader_escapes[NUM_READER_ESCAPES] = {
    {"%d", NULL},
    {"%t", NULL},
    {"%h", NULL}
};

int word_length(char *string)
{
    int i = 0;
    for (i = 0; string[i]; i++) {
        if ((string[i] == ' ') || (string[i] == '\n'))
            break;
    }

    return i;
}

int linect(char *string, int *width)
{
    int i = 0;
    int offset = 0;
    int ct = 0;
    int lines = 1;

    while (string[i]) {
        if (string[i] == '\n') {
            lines++;
            if (offset > *width)
                *width = offset;
            offset = 0;
            i++;
        } else if (string[i] == ' ')
            i++;

        ct = word_length(&string[i]);
        offset += ct + 1;

        if (offset > ni->width - 2) {
            lines += 1 + (ct / (ni->width - 2));
            offset = (ct % (ni->width - 2));
            *width = ni->width - 2;
        }

        i += ct;
    }

    if (offset > *width)
        *width = offset;

    return lines;
}

void reader(char *string)
{
    int lines = 0;
    int width = 0;
    char *escaped = NULL;
    char *filtered = NULL;
    char *descr = NULL;

    if (!string)
        return;

    descr =
        get_cache_descr(ni->selitem->descr_start, ni->selitem->descr_len);
    filtered = filter_description(descr);

    reader_escapes[0].replacement = filtered ? filtered : descr;
    reader_escapes[1].replacement = ni->selitem->title;
    reader_escapes[2].replacement = ni->selfeed->handle;

    escaped = escape_string(string, reader_escapes, NUM_READER_ESCAPES);

    lines = linect(escaped, &width);

    /* Floating placement logic */
    /* Calculate some courtesy variables */

    int realheight = ni->height - 5;
    int realidx = (ni->selfeed->line + ni->selitem->idx - ni->offset - 1);
    int realpos = (realidx) % realheight + 1;
    int realwidth;

    /* Realwidth is the width to the left of the reader */
    /* If it's the first column, pin it to the left */
    if ((realidx / realheight) == 0)
        realwidth = 0;
    /* If it's in the middle, center it on screen */
    else if ((realidx / realheight) == ni->columns - 1)
        realwidth = ni->width - width - 2;
    /* If it's in the last column, pin it to the right */
    else
        realwidth = (ni->width / 2) - (width / 2);


    /* Actually place the reader window. */

    /* Case 1: Plenty of room below */
    if (lines <= realheight - (realpos + 1))
        ni->reader = newwin(lines + 2, width + 2, realpos + 1, realwidth);
    /* Case 2: Plenty of room above */
    else if (lines <= realpos - 1)
        ni->reader =
            newwin(lines + 2, width + 2, (realpos - lines) - 2, realwidth);
    /* Case 3: There's room overlapping  */
    else if (lines <= realheight)
        ni->reader = newwin(lines + 2, width + 2, 0, realwidth);
    /* Case 4: No room, just maximize the window */
    else
        ni->reader = newwin(realheight + 2, width + 2, 0, realwidth);

    wbkgdset(ni->reader, ' ' | COLOR_PAIR(THEME_DEF));
    wattron(ni->reader, COLOR_PAIR(THEME_DEF));
    wclear(ni->reader);

    ni->preader = new_panel(ni->reader);
    theme_print(ni->reader, 1, 1, ni->width - 2, escaped, 1);

    if (lines > (realheight))
        theme_print(ni->reader, realheight, width - 5, ni->width - 2,
                    "...", 0);

    free(descr);
    free(escaped);
    free(filtered);

    wborder(ni->reader, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE,
            ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);

    update_panels();
    doupdate();
}

void reader_off()
{
    del_panel(ni->preader);
    delwin(ni->reader);
    ni->reader = NULL;
    update_panels();
    doupdate();
}
