/* NRSS - ncurses RSS reader
   Copyright (C) 2007 Jack Miller <jjm2n4@umr.edu>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 as 
   published by the Free Software Foundation.
*/
#include "nrss.h"
#include "interface.h"

void update(struct feed *curfeed)
{
    logit("Init update of %s\n", curfeed->URL);
    if (!offline_mode()) {
        curfeed->update_pid = wget(curfeed->URL, curfeed->handle);
        logit("%s - %d\n", curfeed->handle, curfeed->update_pid);
    } else {
        parse_feed(curfeed);
        enumerate(curfeed, curfeed->line);
        interface_draw();
    }
}

void fetched()
{
    int status;
    pid_t pid = 0;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        foreach_feed(curfeed) {
            if (pid == curfeed->update_pid) {
                if (status == 0) {
                    status_message("Updated %s", curfeed->handle);
                    logit("Updated %s\n", curfeed->handle);
                    parse_feed(curfeed);
                    enumerate(curfeed, curfeed->line);
                } else
                    status_message("Error Updating %s", curfeed->URL);

                curfeed->update_pid = 0;
                return;
            }
        }
    }
}

void tick_feeds()
{
    foreach_feed(curfeed) {
        curfeed->time--;
        if (!curfeed->time) {
            update(curfeed);
            curfeed->time = curfeed->rate;
        }
    }
}

void wnch_handler()
{
    logit("SIGWNCH\n");
    endwin();
    refresh();
    rfrsh();
    enumerate(nc->feeds, nc->feeds->line);
    interface_draw();
    unset_wnch();
}

void wnch(int val)
{
    nc->bits |= CFG_WNCH;
}

void alrm_handler()
{
    logit("SIGALRM\n");
    tick_feeds();
    alarm(60);
    unset_alarm();
}

void alrm(int val)
{
    nc->bits |= CFG_ALRM;
}

void chld_handler()
{
    fetched();
    interface_draw();
    unset_child();
}

void chld(int val)
{
    set_child();
}

void pipe_handler()
{
    status_message
        ("Pipe failed. Is your filter command a valid executable?\n");
    unset_pipe();
}

void pype(int val)
{
    nc->bits |= CFG_PIPE;
}
