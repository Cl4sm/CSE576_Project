#ifndef INTERFACEH
#define INTERFACEH

#define IFACE_ALL_SHRUNK 1
#define IFACE_READER 2
#define IFACE_ALL_SOFT_SHRUNK 4

struct niface {
	int height, width;
	
	WINDOW **list;
	WINDOW *status;
	WINDOW *reader;
	WINDOW *usage;

	PANEL **plist;
	PANEL *pstatus;
	PANEL *preader;
	PANEL *pusage;

	struct feed *selfeed;
	struct item *selitem;
	
	int bits;
  int columns;

	int offset;
	int max_offset;
};

struct niface *ni;

/*Interface.c*/
void interface_init();
void interface(int columns);
void interface_destroy();
void rfrsh();

/*Interface.c Bindings*/
void select_next_item();
void select_prev_item();
void handle_goto();
void refresh_cur();
void shrink_all_feeds();
void soft_shrink_all_feeds();
void handle_default();
void handle_expand();
void select_next_feed();
void select_prev_feed();
void handle_usage();
void refresh_all();
void mark_all_read();

/*Reader.c*/
void reader(char *string);
void reader_off();
int word_length(char *string);

/*Usage.c*/
void usage();
void usage_off();

/*Interface_draw.c*/
void interface_draw();
void print_item(struct feed *curfeed, struct item *curitem, WINDOW *win, int y, int x);
void print_title(struct feed *curfeed, WINDOW *win, int y, int x);
void theme_print(WINDOW *win, int y, int x, int width, char *message, int mode);
void enumerate(struct feed *first, int line);
#endif
