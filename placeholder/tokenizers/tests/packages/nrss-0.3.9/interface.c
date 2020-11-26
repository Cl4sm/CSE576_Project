/* NRSS - ncurses RSS reader
   Copyright (C) 2007 Jack Miller <jjm2n4@umr.edu>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 as 
   published by the Free Software Foundation.
*/
#include "nrss.h"
#include "interface.h"

void status_message(char *format, ...)
{
    char *buffer = NULL;
    va_list args;

    if (muted())
        return;

    buffer = xmalloc(ni->width - 1);

    va_start(args, format);

    clear_status();
    vsnprintf(buffer, ni->width - 2, format, args);
    mvwaddstr(ni->status, 1, 1, buffer);

    va_end(args);

    free(buffer);

    update_panels();
    doupdate();
}

void init_themes()
{
    int i = 0;
    for (i = 1; i <= COLORS; i++)
        init_pair(i, nc->themes[i - 1].fg, nc->themes[i - 1].bg);
}

void clean_windows()
{
    int i = 0;

    for (i = 0; i < ni->columns; i++) {
        del_panel(ni->plist[i]);
        delwin(ni->list[i]);
    }

    del_panel(ni->pstatus);
    delwin(ni->status);

    if (reader_open())
        reader_off();
    if (usage_open())
        usage_off();

}

void build_windows(int height, int width)
{
    int i = 0;

    for (i = 0; i < ni->columns; i++) {
        ni->list[i] =
            newwin(height - 3, width / ni->columns, 0,
                   (width / ni->columns) * i);
        ni->plist[i] = new_panel(ni->list[i]);
    }

    ni->status = newwin(3, width, height - 3, 0);
    ni->pstatus = new_panel(ni->status);
    halfdelay(5);

    ni->width = width;
    ni->height = height;

    if (reader_open())
        reader(nc->descr_string);
    if (usage_open())
        usage();
}

void rfrsh()
{
    int nheight, nwidth, i;

    getmaxyx(stdscr, nheight, nwidth);

    if ((ni->height != nheight) || (ni->width != nwidth)) {
        clean_windows();
        build_windows(nheight, nwidth);
    }

    init_themes();

    for (i = 0; i < ni->columns; i++) {
        wbkgdset(ni->list[i], ' ' | COLOR_PAIR(THEME_DEF));
        wattron(ni->list[i], COLOR_PAIR(THEME_DEF));
        wclear(ni->list[i]);
    }

    wattron(ni->status, COLOR_PAIR(THEME_DEF));
    wclear(ni->status);

    wborder(ni->status, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE,
            ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);

    clear_status();

    keypad(ni->status, 1);
    update_panels();
    doupdate();
}

void interface_init()
{
    initscr();
    cbreak();
    noecho();

    start_color();
    rfrsh();
}

void check_scroll_down()
{
    if (ni->selitem) {
        if ((ni->selfeed->line + ni->selitem->idx) >
            ni->offset + (ni->height - 5) * ni->columns)
            ni->offset = ni->selfeed->line +
                ni->selitem->idx - (ni->height - 5) * ni->columns;
    } else {
        if (ni->selfeed->line == 1)
            ni->offset = 0;
        else if (ni->selfeed->line >
                 ni->offset + (ni->height - 5) * ni->columns)
            ni->offset =
                (ni->selfeed->line) - (ni->height - 5) * ni->columns;
    }
}

void check_scroll_up()
{
    if (ni->selitem) {
        if (((ni->selfeed->line + ni->selitem->idx) <= ni->offset))
            ni->offset = (ni->selfeed->line + ni->selitem->idx) - 1;
        else if (ni->selfeed->line + ni->selitem->idx >
                 (ni->offset + ni->height - 5) * ni->columns)
            ni->offset =
                ni->selfeed->line + ni->selitem->idx - (ni->height -
                                                        5) * ni->columns +
                1;
    } else {
        if (ni->selfeed->line <= ni->offset)
            ni->offset = ni->selfeed->line - 1;
    }
}

void mark_read(struct feed *fd, struct item *it)
{
    if (nw(it)) {
        fd->nitems_new--;
        fd->nitems_unread--;
    } else if (unrd(it))
        fd->nitems_unread--;
    
    icachebad(it);
    usetnw(fd, it);
    usetunrd(fd, it);
}

void mark_all_read()
{
    foreach_feed(curfeed) {
        foreach_item(curfeed, curitem)
            mark_read(curfeed, curitem);
    }

    interface_draw();
}

void new_reader()
{
    if ((reader_open()) && (ni->selitem)) {
        reader_off();
        ni->reader = NULL;
        reader(nc->descr_string);
        mark_read(ni->selfeed, ni->selitem);
    }
}

void handle_usage()
{
    if (usage_open())
        usage_off();
    else
        usage();
}

void next_feed()
{
    if (ni->selfeed != nc->feeds->prev) {
        fcachebad(ni->selfeed);
        ni->selfeed = ni->selfeed->next;
        fcachebad(ni->selfeed);
        if(ni->selitem) {
            icachebad(ni->selitem);
            ni->selitem = NULL;
        }
    }
}

void prev_feed()
{
    if (ni->selfeed != nc->feeds) {
        fcachebad(ni->selfeed);
        ni->selfeed = ni->selfeed->prev;
        fcachebad(ni->selfeed);
        if(ni->selitem) {
            icachebad(ni->selitem);
            ni->selitem = NULL;
        }
    }
}

void select_next_feed()
{
    next_feed();
    check_scroll_down();
    interface_draw();
}

void select_prev_feed()
{
    prev_feed();
    check_scroll_up();
    interface_draw();
}

void select_next_item()
{
    if (ni->selitem) {
        icachebad(ni->selitem);
        if ((!ni->selitem->next) ||
            (ni->selitem->next->idx > ni->selfeed->nitems_visible)) {
            next_feed();
        } else {
            icachebad(ni->selitem->next);
            ni->selitem = ni->selitem->next;
        }
    } else if (ni->selfeed) {
        if (ni->selfeed->nitems_visible >= 1) {
            ni->selitem = ni->selfeed->items;
            icachebad(ni->selitem);
        } else if (ni->selfeed->next)
            select_next_feed();
    }

    check_scroll_down();
    interface_draw();
    new_reader();
}

void select_prev_item()
{
    if (ni->selitem) {
        icachebad(ni->selitem);
        if (ni->selitem->idx == 1)
            ni->selitem = NULL;
        else {
            icachebad(ni->selitem->prev);
            ni->selitem = ni->selitem->prev;
        }
    } else if (ni->selfeed != nc->feeds) {
        prev_feed();
        if (ni->selfeed->nitems_visible) {
            ni->selitem = ni->selfeed->items;
            for (int i = 0; i < ni->selfeed->nitems_visible - 1; i++)
                ni->selitem = ni->selitem->next;
            icachebad(ni->selitem);
        }
    }

    check_scroll_up();
    interface_draw();
    new_reader();
}

void handle_goto()
{
    if (ni->selitem) {
        open_url(ni->selitem->link);
        mark_read(ni->selfeed, ni->selitem);

    } else
        open_url(ni->selfeed->link);
}


void refresh_feed(struct feed *cfd)
{
    if (!cfd->update_pid) {
        update(cfd);
        cfd->time = cfd->rate;
    }
}

void refresh_cur()
{
    refresh_feed(ni->selfeed);
}

void refresh_all()
{
    foreach_feed(curfeed)
        refresh_feed(curfeed);
}

void toggle_shrunk(struct feed *cur)
{
    if (sh(cur)) {
        usetsh(cur);
    } else {
        setsh(cur);
    }

    enumerate(cur, cur->line);
}

void expand_feed(struct feed *cur)
{
    if (ex(cur)) {
        usetex(cur);
    } else {
        setex(cur);
    }

    enumerate(cur, cur->line);
}

void shrink_all_feeds()
{
    if (all_shrunk())
        ni->bits &= ~(IFACE_ALL_SHRUNK);
    else
        ni->bits |= IFACE_ALL_SHRUNK;

    if (ni->selitem)
        icachebad(ni->selitem);

    foreach_feed(curfeed)
        fcachebad(curfeed);

    ni->selitem = NULL;
    enumerate(nc->feeds, nc->feeds->line);
    ni->offset = ni->selfeed->line - 1;
    interface_draw();
}

void soft_shrink_all_feeds()
{
    if(all_soft_shrunk()) {
        ni->bits &= ~(IFACE_ALL_SOFT_SHRUNK);
        foreach_feed(curfeed) {
            usetsh(curfeed);
            fcachebad(curfeed);
        }
    }
    else {
        ni->bits |= IFACE_ALL_SOFT_SHRUNK;
        foreach_feed(curfeed) {
            setsh(curfeed);
            fcachebad(curfeed);
        }
    }

    if(ni->selitem)
        icachebad(ni->selitem);

    ni->selitem = NULL;
    enumerate(nc->feeds, nc->feeds->line);
    ni->offset = ni->selfeed->line - 1;
    interface_draw();
}

void handle_default()
{
    if (ni->selitem) {
        if (reader_open()) {
            reader_off();
            ni->reader = NULL;
        } else {
            mark_read(ni->selfeed, ni->selitem);
            reader(nc->descr_string);
        }
    } else
        toggle_shrunk(ni->selfeed);

    interface_draw();
}

void handle_expand()
{
    expand_feed(ni->selfeed);
    while ((ni->selitem)
           && (ni->selitem->idx > ni->selfeed->nitems_visible))
        select_prev_item();
    interface_draw();
}

void interface(int columns)
{
    unsigned int i = 0;
    unsigned int key = 0;

    ni = xmalloc(sizeof(struct niface));
    memset(ni, 0, sizeof(struct niface));

    ni->list = xmalloc(sizeof(WINDOW *) * columns);
    memset(ni->list, 0, sizeof(WINDOW *) * columns);

    ni->plist = xmalloc(sizeof(PANEL *) * columns);
    memset(ni->plist, 0, sizeof(WINDOW *) * columns);

    ni->columns = columns;
    ni->offset = 1;

    interface_init();
    enumerate(nc->feeds, 1);

    ni->selfeed = nc->feeds;

    interface_draw();
    tick_feeds();

    if (!ni->selfeed)
        status_message("Add some feeds! (read 'man nrss')");

    while (1) {
        key = wgetch(ni->status);

        if (key != -1) {
            if (key == 27)
                key = ALT(wgetch(ni->status));
            for (i = 0; i < (ni->selfeed ?
                             NUM_FBINDINGS : NUM_BINDINGS); i++) {
                if (nc->keys[i].bind == key)
                    nc->keys[i].function();
            }
        }

        if (signal_child())
            chld_handler();
        if (signal_alarm())
            alrm_handler();
        if (signal_wnch())
            wnch_handler();
        if (signal_pipe())
            pipe_handler();
    }
}

void interface_destroy()
{
    clean_windows();
    free(ni->list);
    free(ni->plist);
    free(ni);
    endwin();
}
