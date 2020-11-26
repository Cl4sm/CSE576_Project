/* NRSS - ncurses RSS reader
   Copyright (C) 2007 Jack Miller <jjm2n4@umr.edu>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 as 
   published by the Free Software Foundation.
*/
#include "nrss.h"
#include "interface.h"

#define NUM_TITLE_ESCAPES 7
static struct escape title_escapes[NUM_TITLE_ESCAPES] = {
    {"%u", NULL},
    {"%i", NULL},
    {"%n", NULL},
    {"%d", NULL},
    {"%r", NULL},
    {"%h", NULL},
    {"%t", NULL}
};

#define NUM_STORY_ESCAPES 1
static struct escape story_escapes[NUM_STORY_ESCAPES] = {
    {"%s", NULL}
};

#define NUM_TITLE_TERNS 2
static struct ternary title_terns[NUM_TITLE_TERNS] = {
    {'x', 0},
    {'s', 0}
};

#define NUM_STORY_TERNS 4
static struct ternary story_terns[NUM_STORY_TERNS] = {
    {'e', 0},
    {'b', 0},
    {'s', 0},
    {'f', 0}
};

#define BIT_BOLD 1
#define BIT_UNDERLINE 2
#define BIT_STANDOUT 4
#define BIT_REVERSE 8
#define BIT_DIM 16

#define toggle_att(att) \
	if(on & BIT_##att) {\
		wattroff(win, A_##att);\
		on = on & ~(BIT_##att);\
	}\
	else {\
		wattron(win, A_##att);\
		on = on | BIT_##att;\
	}

#define MODE_NOWRAP 0
#define MODE_WRAP 1

void theme_print(WINDOW * win, int y, int x, int width, char *message,
                 int mode)
{
    int on = 0;
    int i = 0;
    int ct = 0;
    int print = 1;

    if (!message)
        return;

    wmove(win, y, x);
    for (i = 0; message[i] != 0; i++, x++) {

        if (((ct == 0) || (message[i] == ' ')) && (mode == MODE_WRAP)) {
            ct = word_length(&message[i + 1]);
            if (x + ct > ni->width - 2) {
                x = 0;
                y++;
            }
        }

        if ((x > width) || (message[i] == '\n')) {
            if (mode == MODE_WRAP) {
                y++;
                x = 0;
            } else
                print = 0;
        }

        if (message[i] == '\\') {
            i++;
            if (print)
                mvwaddch(win, y, x, message[i]);
        } else if (message[i] == '%') {
            i++;
            x--;

            if (message[i] == 0)
                break;

            if (message[i] == 'B') {
                toggle_att(BOLD);
            } else if (message[i] == 'U') {
                toggle_att(UNDERLINE)
            } else if (message[i] == 'S') {
                toggle_att(STANDOUT)
            } else if (message[i] == 'R') {
                toggle_att(REVERSE)
            } else if (message[i] == 'D') {
                toggle_att(DIM)
            } else if ((message[i] >= '1') && (message[i] <= '8'))
                wattron(win, COLOR_PAIR(message[i] - '0'));
        }
#ifdef NCURSESW
        else if ((unsigned char) message[i] > 0x7F) {
            wchar_t dest[2];
            int bytes = 0;

            i +=  mbtowc(dest, &message[i], 4) - 1;
            dest[1] = 0;
            bytes = wcswidth(dest, 1);
            if((x + bytes <= width)&&(print))
                mvwaddwstr(win, y, x, dest);
            x += wcswidth(dest, 1) - 1;
        }
#endif
        else if (print)
            mvwaddch(win, y, x, message[i]);

    }

    wclrtoeol(win);
    wattron(win, COLOR_PAIR(THEME_DEF));
}

void print_title(struct feed *curfeed, WINDOW * win, int y, int x)
{
    char *tern_string = NULL;
    char itemstr[5];
    char nitemstr[5];
    char ditemstr[5];
    char ritemstr[5];

    if (!fcached(curfeed)) {
        if (curfeed->cached_line)
            free(curfeed->cached_line);
        fcacheok(curfeed);
        snprintf(itemstr, 5, "%d", curfeed->nitems);
        snprintf(nitemstr, 5, "%d", curfeed->nitems_new);
        snprintf(ditemstr, 5, "%d", curfeed->nitems_unread);
        snprintf(ritemstr, 5, "%d",
                 curfeed->nitems ? curfeed->nitems -
                 curfeed->nitems_unread - 1 : 0);

        title_escapes[0].replacement = curfeed->URL;
        title_escapes[1].replacement = itemstr;
        title_escapes[2].replacement = nitemstr;
        title_escapes[3].replacement = ditemstr;
        title_escapes[4].replacement = ritemstr;
        title_escapes[5].replacement = curfeed->handle;
        title_escapes[6].replacement = curfeed->title;

        title_terns[0].status = ((!sh(curfeed)) && (!all_shrunk()));
        title_terns[1].status = (curfeed == ni->selfeed);

        tern_string = ternary_string(nc->title_string,
                                     title_terns, NUM_TITLE_TERNS);

        curfeed->cached_line = escape_string(tern_string,
                                             title_escapes,
                                             NUM_TITLE_ESCAPES);
    }

    theme_print(win, y, x, ni->width / ni->columns - 2,
                curfeed->cached_line, MODE_NOWRAP);

    free(tern_string);
}

char *get_item_string(struct item *cur)
{
    if (nw(cur))
        return nc->story_string;
    else if (unrd(cur))
        return nc->u_story_string;
    else
        return nc->r_story_string;
}

void print_item(struct feed *curfeed, struct item *curitem, WINDOW * win,
                int y, int x)
{
    char *tern_string = NULL;

    if (!icached(curitem)) {
        if (curitem->cached_line)
            free(curitem->cached_line);
        icacheok(curitem);
        story_escapes[0].replacement = curitem->title;

        story_terns[0].status = (curitem->idx == curfeed->nitems_visible);
        story_terns[1].status = (curitem->idx == 1);
        story_terns[2].status = (curitem == ni->selitem);
        story_terns[3].status = (curfeed == ni->selfeed);

        tern_string = ternary_string(get_item_string(curitem),
                                     story_terns, NUM_STORY_TERNS);

        curitem->cached_line = escape_string(tern_string,
                                             story_escapes,
                                             NUM_STORY_ESCAPES);
    }

    theme_print(win, y, x, ni->width / ni->columns - 2,
                curitem->cached_line, MODE_NOWRAP);

    free(tern_string);
}

void calc_visible(struct feed *cur)
{
    if ((all_shrunk()) || (sh(cur)))
        cur->nitems_visible = 0;
    else if (ex(cur))
        cur->nitems_visible = max(cur->nitems - 1, 0);
    else
        cur->nitems_visible = min(max(cur->nitems - 1, 0), cur->show);
}

void enumerate(struct feed *first, int line)
{
    struct feed *cur = NULL;
    int idx = 1;

    for (cur = first; cur; cur = cur->next) {
        idx = 1;

        foreach_item(cur, curitem)
            curitem->idx = idx++;

        calc_visible(cur);
        cur->line = line;
        line += cur->nitems_visible + 2;
    }

    ni->max_offset = max(line - ((ni->height - 4) * ni->columns), 0);

}

void interface_draw()
{
    int curwin = 0;
    int i = 1;

    if (muted())
        return;

    ni->offset = min(ni->max_offset, ni->offset);
    foreach_feed(curfeed) {

        if (curfeed->line + curfeed->nitems_visible >= ni->offset) {

            if (curfeed->line > ni->offset)
                print_title(curfeed, ni->list[curwin], i++, 1);

            if (i > ni->height - 5) {
                if (curwin == (ni->columns - 1))
                    break;
                curwin++;
                i = 1;
            }

            foreach_visible_item(curfeed, curitem) {
                if (curfeed->line + curitem->idx > ni->offset)
                    print_item(curfeed, curitem, ni->list[curwin], i++, 1);

                if (i > ni->height - 5) {
                    if (curwin == (ni->columns - 1))
                        goto done;
                    curwin++;
                    i = 1;
                }
            }

            theme_print(ni->list[curwin], i++, 0,
                        ni->width / ni->columns - 2, "", MODE_NOWRAP);

            if (i > ni->height - 5) {
                if (curwin == (ni->columns - 1))
                    break;
                curwin++;
                i = 1;
            }
        }
    }

  done:
    wclrtobot(ni->list[curwin]);

    for (i = curwin + 1; i < ni->columns; i++)
        wclear(ni->list[i]);

    for (i = 0; i < ni->columns; i++)
        wborder(ni->list[i], ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE,
                ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);

    update_panels();
    doupdate();
}
