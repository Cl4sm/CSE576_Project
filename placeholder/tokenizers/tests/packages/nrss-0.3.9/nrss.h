#ifndef NRSSH
#define NRSSH

#define _GNU_SOURCE
#include <locale.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <expat.h>
#include <stdio.h>

#ifdef NCURSESW
 #include <ncursesw/ncurses.h>
 #include <ncursesw/panel.h>
#elif NCURSES
 #include <ncurses.h>
 #include <panel.h>
#endif

#include "macros.h"

#ifndef WGET
#ifndef WGET_PATH
 #define WGET "/usr/bin/wget -q -O \"%p\" -U %a %u"
#else
 #define WGET WGET_PATH " -q -O \"%p\" -U %a %u"
#endif
#endif

#ifndef BUFFER
#define BUFFER 1024
#endif

struct key {
	int bind;
	void (*function)(void);
	char *descr;
	char *printable;
};

struct escape {
	char *ident;
	char *replacement;
};

struct ternary {
	char ident;
	int status;
};

#define ITEM_NEW 1
#define ITEM_UNREAD 2
#define ITEM_CACHED 4

struct item {
	struct item *next;
	struct item *prev;

    char *cached_line;
	char *title;
	char *link;

    int descr_start;
    int descr_len;

	int bits;
	int idx;
};


#define FEED_SHRUNK 1
#define FEED_EXP 2
#define FEED_CACHED 4

struct feed {
	struct feed *next;
	struct feed *prev;

	char *handle;
	char *URL;
	char *title;
	char *link;
    char *cached_line;

	pid_t update_pid;

	int time;

	int line;
	int rate;
	int show;
	int keep; 

	int bits;

	int nitems;
	int nitems_new;
	int nitems_unread;
	int nitems_visible;

	struct item *items;
	struct item *olditems;
};

struct theme {
	int fg;
	int bg;
};

#define CTRL_MASK 0x1F
#define ALT_BITS 0x10000
#define ALT(_x) (_x | ALT_BITS)
#define CTRL(_x) (_x & CTRL_MASK)

#define NUM_DEFS 4

#define COLORS 8
#define THEME_DEF 1

#define NUM_BINDINGS 2
#define NUM_FBINDINGS 15

#define DEF_RATE 0
#define DEF_SHOW 1
#define DEF_MAX 2
#define DEF_COLS 3

#define CFG_BITS_BROWSER_WAIT 1
#define CFG_MUTED 2
#define CFG_OFFLINE 4
#define CFG_ALRM 8
#define CFG_PIPE 16
#define CFG_WNCH 32
#define CFG_SETCOLS 64

struct ncfg {
    /*Files*/
	char *config_dir;
	char *feed_dir;
	char *config_file;
	char *log_file;
    char *cache_file;

    /*Fork lines*/
	char *browser_line;
	char *wget_line;
	char *desc_filter_line;
	char *story_filter_line;

	/*Escapes*/
	char *title_string;
	char *story_string;
	char *r_story_string;
	char *u_story_string;
	char *descr_string;

	int bits;
    int children;

	int defs[NUM_DEFS];
	struct theme themes[COLORS];
	struct key keys[NUM_BINDINGS];
	struct key fkeys[NUM_FBINDINGS - NUM_BINDINGS];
	struct feed *feeds;
};

extern char **environ;
struct ncfg *nc;

/*Args.c*/
void parse_args(int argc, char **args);

/*List.c*/
void ladd_item(struct item **head, struct item *newitem);
void lfree_item(struct item *head);
void ladd_feed(struct feed **head, struct feed *newfeed);
void lfree_feed(struct feed *head);
int lsearch(struct item *items, char *title);

/*Commands.c*/
pid_t wget(char *URL, char *fname);
void open_url(char *URL);
char *filter_string(char *str, char *cmd);

/*Utility.c*/
char *ternary_string(char *format, struct ternary *terns, int num_terns);
char *escape_string(char *format, struct escape *escapes, int num_esc);
char *concat(char *one, char *two);
char *xstrdup(const char *s);
char *xstrndup(const char *s, size_t n);
void *xmalloc(size_t n);

/*Main.c (signals)*/
void cleanup();

/*System.c*/
void update(struct feed *curfeed);
void fetched();

void alrm();
void alrm_handler();

void wnch();
void wnch_handler();

void chld();
void chld_handler();

/* Ye old pype handler.  */
void pype();
void pipe_handler();

void tick_feeds();

/*Log.c*/
void logit(char *format, ...);

/*Parse.c*/
void parse_feed(struct feed *curfeed);

/*Interface.c*/
void status_message(char *message, ...);

/*Filter.c*/
char *filter_description(char *str);
char *read_string_from_fd(int source);
void write_string_to_fd(const char *str, int dest);

/*Cache.c*/
char *get_cache_descr(int start, int len);
void desc_cache(char *descr, int *start, int *len);
#endif
