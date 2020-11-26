/* NRSS - ncurses RSS reader
   Copyright (C) 2007 Jack Miller <jjm2n4@umr.edu>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 as 
   published by the Free Software Foundation.
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>

#include "cnf.h"

#ifdef BSD

int getline(char **line, size_t * len, FILE * fp)
{
    size_t mylen;
    char *buf;

    buf = fgetln(fp, &mylen);
    if (!mylen)
        return -1;

    if (!buf)
        return -1;

    if (buf[mylen - 1] == '\n') {
        buf[mylen - 1] = 0;
        *line = malloc(mylen);
        memcpy(*line, buf, mylen);
        *len = mylen;
        return mylen;
    } else {
        *line = malloc(mylen + 1);
        (*line)[mylen] = 0;
        memcpy(*line, buf, mylen);
        *len = mylen + 1;
        return mylen + 1;
    }
}
#endif

static struct cnf_config_list *cfg_list;

void fake_cnfdbg(char *format, ...)
{
}

/*Return a class based on the identifying */
struct cnf_config_class *get_class(char ident, struct cnf_config *cfg)
{
    int i = 0;
    for (i = 0; i < cfg->num_classes; i++) {
        if (cfg->classes[i].ident == ident)
            return &cfg->classes[i];
    }

    return NULL;
}

void *get_data(char *format, char *line, struct cnf_config *cfg)
{
    struct cnf_config_class *cls;
    mac_addr *data = NULL;
    int i = 0;
    int j = 0;
    void *tok = NULL;

    data = malloc(sizeof(void *) * strlen(format));


    for (i = 0; format[i] != 0; i++) {
        cls = get_class(format[i], cfg);
        if (!cls) {
            cnfdbg("Unknown class in config meta\n");
            break;
        }

        /*Skip over any whitespace */
        while ((line[j]) && (line[j] != cls->delimiter))
            j++;

        if (line[j])
            tok = &line[++j];
        else
            break;

        /*We can safely reference line[-1] because this
         * is a substring within the entire config line*/
        for (j = j; line[j]; j++) {
            if ((line[j] == cls->delimiter)) {
                if (line[j - 1] != '\\')
                    break;
                else
                    memmove(&line[j - 1], &line[j], strlen(&line[j]));
            }
        }

        if (line[j])
            line[j++] = 0;
        else
            break;

        if (is_numeric(cls))
            data[i] = atoi(tok);
        else
            data[i] = (mac_addr) strdup((char *) tok);

        if ((cls->sanity) && (cls->sanity((void *) data[i]))) {
            cnfdbg("Invalid arg.\n");
            break;
        }
    }

    if (format[i] == 0)
        return data;

    free(data);
    return NULL;
}

/*Uses the struct to determine whether two things are equal*/
int same_item(struct cnf_config_list *item, struct cnf_config_list *item2,
              struct cnf_config *cfg)
{
    struct cnf_config_class *cls = NULL;
    int i = item->meta->ident_arg;

    if (!i)
        return 0;
    else
        i--;

    cls = get_class(item->meta->classes[i], cfg);
    if (((is_numeric(cls))
         && (((int *) item->data)[i] == ((int *) item2->data)[i]))
        || ((!is_numeric(cls))
            &&
            (!strcmp
             (((char **) item->data)[i], ((char **) item2->data)[i]))))
        return 1;

    return 0;
}

/*Returns 0 if duplicate*/
int duplicate(struct cnf_config_list *item, struct cnf_config *cfg)
{
    struct cnf_config_list *cur = cfg_list;

    while (cur != NULL) {
        if ((item->meta == cur->meta)
            && (same_item(cur, item, cfg)))
            return 0;

        cur = cur->next;
    }

    return 1;
}

void add_item(struct cnf_config_list *item)
{
    struct cnf_config_list *cur = cfg_list;

    item->next = NULL;

    if (!cfg_list) {
        cfg_list = item;
        return;
    }

    while (cur->next)
        cur = cur->next;

    cur->next = item;
}

void free_item(struct cnf_config_list *item, char *format,
               struct cnf_config *cfg)
{
    struct cnf_config_class *cls = NULL;
    int i = 0;
    for (i = 0; format[i] != 0; i++) {
        cls = get_class(format[i], cfg);
        if ((cls) && (!is_numeric(cls)))
            free(((char **) item->data)[i]);
    }

    free(item->data);
    free(item);
}

void cnf_free_list(struct cnf_config_list *item, struct cnf_config *cfg)
{
    if (!item)
        return;

    cnf_free_list(item->next, cfg);
    free_item(item, item->meta->classes, cfg);
}

int cnf_parse_line(char *line, struct cnf_config *cfg)
{
    struct cnf_config_list *item = NULL;
    int i = 0;
    char *args = NULL;

    /*Just avoid comments and empty lines */
    if ((line[0] == '#') || (line[0] == '\n'))
        return 0;

    /*Strip the ending newline */
    if (line[strlen(line) - 1] == '\n')
        line[strlen(line) - 1] = 0;

    /*Get the initial identifying token */
    for (i = 0; line[i] != 0; i++) {
        if (line[i] == cfg->delimiter[0]) {
            line[i] = 0;
            args = &line[i + 1];
            break;
        }
    }

    /*Now we handle the class */
    for (i = 0; i < cfg->num_meta; i++) {
        if (!strcmp(line, cfg->metas[i].name)) {
            item = malloc(sizeof(struct cnf_config));
            item->meta = &cfg->metas[i];
            if (cfg->metas[i].classes[0] != '0') {
                item->data = get_data(cfg->metas[i].classes, args, cfg);
                if (!item->data) {
                    free(item);
                    item = NULL;
                }
            }
            break;
        }
    }

    /*Something went wrong if !item */
    if (!item)
        return 1;

    /*Check duplicates */
    if ((!duplicate(item, cfg)) ||
        ((cfg->metas[i].done) && (cfg->metas[i].done(item->data)))) {
        free_item(item, cfg->metas[i].classes, cfg);
        return 1;
    }

    /*Excellent, add the item */
    add_item(item);
    return 0;
}

char *get_next_line(char *fname)
{
    static FILE *fp = NULL;
    char *line = NULL;
    size_t len = 0;

    if ((!fp) && ((fp = fopen(fname, "r")) <= 0))
        return NULL;

    if (getline(&line, &len, fp) < 0) {
        free(line);
        fclose(fp);
        return NULL;
    }

    return line;
}

void cnf_parse(char *fname, struct cnf_config *cfg,
               struct cnf_config_list **cfgl)
{
    char *line = NULL;
    int linect = 0;

    while ((line = get_next_line(fname))) {
        linect++;
        if (cnf_parse_line(line, cfg))
            cnfdbg("[%s][%d] %s\n", fname, linect, "Parse Error");
        free(line);
    }

    if (cfgl)
        *cfgl = cfg_list;
    else
        cnf_free_list(cfg_list, cfg);
}
