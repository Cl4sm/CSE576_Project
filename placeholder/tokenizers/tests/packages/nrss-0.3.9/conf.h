#ifndef CONFH
#define CONFH

/*Feeds.c*/
int add_feed(void *data);
struct feed *get_feed(char *handle);

/*Conf.c*/
int set_max(void *data);
int set_show(void *data);
int set_rate(void *data);

int set_def_max(void *data);
int set_def_show(void *data);
int set_def_rate(void *data);
int set_def(int def, int num);

int set_theme(void *data);
int set_theme_unread(void *data);
int set_theme_new(void *data);
int set_theme_title(void *data);
int set_thm(int theme, int fg, int bg);

int set_browser(void *data);
int set_browser_wait(void *data);
int set_wget(void *data);
int set_desc_filter(void *data);
int set_story_filter(void *data);
int set_title_escape(void *data);
int set_story_escape(void *data);
int set_sel_title_escape(void *data);
int set_sel_story_escape(void *data);
int set_reader_escape(void *data);
int set_r_story_escape(void *data);
int set_u_story_escape(void *data);
int set_sel_r_story_escape(void *data);
int set_sel_u_story_escape(void *data);
int set_columns(void *data);

int check_theme(void *data);

/*Keys.c*/
int set_key(void *data);
int check_char(void *data);


#endif
