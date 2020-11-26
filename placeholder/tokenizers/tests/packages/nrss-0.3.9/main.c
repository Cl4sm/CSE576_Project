/* NRSS - ncurses RSS reader
   Copyright (C) 2007 Jack Miller <jjm2n4@umr.edu>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 as 
   published by the Free Software Foundation.
*/
#include "cnf/cnf.h"
#include "conf.h"
#include "nrss.h"
#include "interface.h"

static struct cnf_config_list *cfg_list = NULL;

#define CLASSES 4
static struct cnf_config_class classes[CLASSES] = {
    {'s', CFG_CLASS_STR, '\"', NULL},
    {'c', CFG_CLASS_STR, '\"', check_char},
    {'i', CFG_CLASS_NUM, '\"', NULL},
    {'t', CFG_CLASS_NUM, '\"', check_theme}
};

#define METAS 20
static struct cnf_config_meta metas[METAS] = {
    {"add", "ss", 2, add_feed},
    {"rate", "is", 2, set_rate},
    {"show", "is", 2, set_show},
    {"maxitems", "is", 2, set_max},
    {"default_maxitems", "i", 0, set_def_max},
    {"default_rate", "i", 0, set_def_rate},
    {"default_show", "i", 0, set_def_show},
    {"theme", "itt", 1, set_theme},
    {"browser_wait", "i", 0, set_browser_wait},
    {"browser", "s", 0, set_browser},
    {"wget", "s", 0, set_wget},
    {"description_filter", "s", 0, set_desc_filter},
    {"title_filter", "s", 0, set_story_filter},
    {"title_string", "s", 0, set_title_escape},
    {"n_story_string", "s", 0, set_story_escape},
    {"u_story_string", "s", 0, set_u_story_escape},
    {"r_story_string", "s", 0, set_r_story_escape},
    {"reader_string", "s", 0, set_reader_escape},
    {"columns", "i", 0, set_columns},
    {"key", "sc", 0, set_key}
};

static struct cnf_config cfg = { " ", METAS, CLASSES, metas, classes };

void cleanup_paths()
{
    free(nc->config_dir);
    free(nc->config_file);
    free(nc->log_file);
    free(nc->feed_dir);
    free(nc->cache_file);
}

void cleanup()
{
    cleanup_paths();
    lfree_feed(nc->feeds);
    cnf_free_list(cfg_list, &cfg);
    interface_destroy();
    exit(0);
}

struct ncfg my_nc = {
    NULL, NULL, NULL, NULL, NULL, NULL, WGET, NULL, NULL,
    " %?s(>: ) %U%B%h%U %?x(-:+) [%2%n%1][%3%d%1]%B",
    " %?s(>: ) %B%2%s%1%B",
    " %?s(>: ) %2%s%1",
    " %?s(>: ) %3%s%1",
    "%B%t%B\n\n%d",
    0, 0,
    {5, 30, 50, 1},
    {{COLOR_WHITE, COLOR_BLACK},
     {COLOR_BLUE, COLOR_BLACK},
     {COLOR_GREEN, COLOR_BLACK},
     {COLOR_RED, COLOR_BLACK}},
    {{'q', cleanup, "quit", "q"},
     {'h', handle_usage, "toggle-usage", "h"}},
    {{KEY_DOWN, select_next_item, "next", "KEY_DOWN"},
     {KEY_UP, select_prev_item, "prev", "KEY_UP"},
     {KEY_NPAGE, select_next_feed, "next-feed", "KEY_NPAGE"},
     {KEY_PPAGE, select_prev_feed, "prev-feed", "KEY_PPAGE"},
     {'g', handle_goto, "goto", "g"},
     {'r', refresh_cur, "refresh", "r"},
     {'R', refresh_all, "refresh-all", "R"},
     {' ', handle_default, "default", "Space"},
     {'C', shrink_all_feeds, "toggle-collapse-all", "C"},
     {'x', handle_expand, "toggle-expand", "x"},
     {'M', mark_all_read, "mark-all-read", "M"},
     {'D', wnch_handler, "redraw", "D"},
     {0, soft_shrink_all_feeds, "toggle-set-collapse-all", ""}
    },

    NULL
};

int main(int argc, char **argv)
{
    char *home = getenv("HOME");
    int ret = 0;

    setlocale(LC_ALL, "");

    nc = &my_nc;

    parse_args(argc, argv);

    safeset(nc->config_dir, concat(home, "/.nrss/"));
    safeset(nc->feed_dir, concat(nc->config_dir, "feeds/"));
    safeset(nc->config_file, concat(nc->config_dir, "config"));
    safeset(nc->log_file, concat(nc->config_dir, "log"));
    nc->cache_file = concat(nc->config_dir, ".cache");

    ret = unlink(nc->cache_file);
    if ((ret) && (errno != ENOENT)) {
        cleanup_paths();
        printf("Unlink failed: %s\n", strerror(errno));
        exit(-1);
    }

    ret = unlink(nc->log_file);
    if ((ret) && (errno != ENOENT)) {
        cleanup_paths();
        printf("Unlink failed: %s\n", strerror(errno));
        exit(-1);
    }

    ret = mkdir(nc->config_dir, 0755);
    if ((ret != 0) && (errno != EEXIST)) {
        logit("Couldn't create config dir: %s\n", strerror(errno));
        cleanup_paths();
        exit(-1);
    }

    ret = mkdir(nc->feed_dir, 0755);
    if ((ret != 0) && (errno != EEXIST)) {
        logit("Couldn't create feed dir: %s\n", strerror(errno));
        cleanup_paths();
        exit(-1);
    }

    logit("NRSS v%s\n", VERSION);

    cnf_parse(nc->config_file, &cfg, &cfg_list);

    signal(SIGALRM, alrm);
    signal(SIGWINCH, wnch);
    signal(SIGINT, cleanup);
    signal(SIGCHLD, chld);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGPIPE, pype);
    alarm(60);

    interface(nc->defs[DEF_COLS]);
    cleanup();

    return 0;
}
