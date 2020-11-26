/* NRSS - ncurses RSS reader
   Copyright (C) 2007 Jack Miller <jjm2n4@umr.edu>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 as 
   published by the Free Software Foundation.
*/
#include "nrss.h"
#include "interface.h"

void usage()
{
    int i = 0;
    int y = 0;
    int bound = NUM_FBINDINGS;
    int len = 0;

    int max = 0;
    int pmax = 0;

    for (i = 0; i < NUM_FBINDINGS; i++) {
        logit("Bind: %d\n", nc->keys[i].bind);
        if(nc->keys[i].bind != 0) {
            len = strlen(nc->keys[i].descr);
            if (len > max)
                max = len;

            len = strlen(nc->keys[i].printable);
            if (len > pmax)
                pmax = len;
        }
        else
            bound--;
    }

    ni->usage = newwin(bound + 2, max + pmax + 5,
                       ni->height - bound - 5,
                       ni->width - (max + pmax + 6));

    wbkgdset(ni->usage, ' ' | COLOR_PAIR(THEME_DEF));
    wattron(ni->usage, COLOR_PAIR(THEME_DEF));
    wclear(ni->usage);

    ni->pusage = new_panel(ni->usage);

    for (i = 0; i < NUM_FBINDINGS; i++) {
        if(nc->keys[i].bind != 0) {
            mvwprintw(ni->usage, y + 1,
                    2 + (pmax - strlen(nc->keys[i].printable)),
                    "%s", nc->keys[i].printable);
            mvwprintw(ni->usage, y + 1, 3 + pmax, "%s", nc->keys[i].descr);
        }
        y++;
    }

    wborder(ni->usage, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE,
            ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);

    update_panels();
    doupdate();
}

void usage_off()
{
    del_panel(ni->pusage);
    delwin(ni->usage);
    ni->usage = NULL;
    update_panels();
    doupdate();
}
