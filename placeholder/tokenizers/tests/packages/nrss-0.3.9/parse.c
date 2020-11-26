/* NRSS - ncurses RSS reader
   Copyright (C) 2007 Jack Miller <jjm2n4@umr.edu>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 as 
   published by the Free Software Foundation.
*/
#include "nrss.h"

static struct feed *cfd = NULL;
static struct item *newitem = NULL;
static char *descr = NULL;

static char *current_tag = "NRSS";
static char **ptr = NULL;
static int items_parsed = 0;
static int atom = 0;

void strings(void *data, const XML_Char * s, int len)
{
    int size = 0;
    char *newstr = NULL;

    if (!ptr)
        return;
    else if (!*ptr)
        *ptr = xstrndup(s, len);
    else {
        size = strlen(*ptr) + len + 1;
        newstr = xmalloc(size);
        newstr = strcpy(newstr, *ptr);
        newstr = strncat(newstr, s, len);

        free(*ptr);
        *ptr = newstr;
    }
}

char *getattr(char *name, const char **attr)
{
    int i = 0;

    while (attr[i] != NULL) {
        if (!strcmp(attr[i], name))
            return xstrdup(attr[i + 1]);
        i++;
    }

    return NULL;
}

void start(void *data, const char *el, const char **attr)
{
    if (newitem) {
        if (!strncmp(el, "title", 6)) {
            if (!newitem->title)
                ptr = &newitem->title;
            current_tag = "title";
        } else if (!strncmp(el, "link", 5)) {
            if (!newitem->link) {
                if (atom) {
                    char *tmp = getattr("rel", attr);
                    if((tmp)&&(!strncmp(tmp, "alternate", 10))) {
                        free(tmp);
                        tmp = getattr("href", attr);
                        if(tmp)
                            newitem->link = tmp;
                    }
                    else
                        free(tmp);
                }
                else
                    ptr = &newitem->link;
            }
            current_tag = "link";
        } else if (!strncmp(el, "description", 12)) {
            if (!descr)
                ptr = &descr;
            current_tag = "description";
        } else if (!strncmp(el, "content", 8)) {
            if (!descr)
                ptr = &descr;
            current_tag = "content";
        }
    } else if (!strncmp(el, "item", 5)) {
        newitem = xmalloc(sizeof(struct item));
        memset(newitem, 0, sizeof(struct item));
        current_tag = "item";
        descr = NULL;
        atom = 0;
    } else if (!strncmp(el, "entry", 6)) {
        newitem = xmalloc(sizeof(struct item));
        memset(newitem, 0, sizeof(struct item));
        current_tag = "entry";
        descr = NULL;
        atom = 1;
    } else if (!strncmp(el, "link", 5)) {
        if (!cfd->link)
            ptr = &cfd->link;
        current_tag = "link";
    } else if (!strncmp(el, "title", 6)) {
        if (!cfd->title)
            ptr = &cfd->title;
        current_tag = "title";
    }
}

void end(void *data, const char *el)
{
    if ((newitem)&&((!strncmp(el, "item", 5))||(!strncmp(el, "entry", 6)))) {
        if (((!lsearch(cfd->items, newitem->title))) ||
            (items_parsed >= cfd->keep))
            lfree_item(newitem);
        else {
            if (cfd->nitems >= cfd->keep) {
                cfd->nitems--;
                if (nw(cfd->items->prev))
                    cfd->nitems_new--;
                if (unrd(cfd->items->prev))
                    cfd->nitems_unread--;

                cfd->items->prev = cfd->items->prev->prev;
                lfree_item(cfd->items->prev->next);
                cfd->items->prev->next = NULL;
            }
            setnw(cfd, newitem);
            setunrd(cfd, newitem);

            desc_cache(descr, &newitem->descr_start, &newitem->descr_len);
            free(descr);
            descr = NULL;

            ladd_item(&cfd->items, newitem);

            cfd->nitems++;
            cfd->nitems_new++;
            cfd->nitems_unread++;
        }

        items_parsed++;
        newitem = NULL;
    } else if ((ptr == &newitem->title) && (nc->story_filter_line)) {
        char *filtered_string = NULL;
        filtered_string =
            filter_string(newitem->title, nc->story_filter_line);
        if(filtered_string) {
            free(newitem->title);
            newitem->title = filtered_string;
        }
    }

    if (!strcmp(el, current_tag))
        ptr = NULL;
}

void parse_feed(struct feed *curfeed)
{
    FILE *fp = NULL;
    char *buff = xmalloc(BUFFER);
    char *filename = NULL;
    int len = 0;
    int error = 0;

    cfd = curfeed;
    cfd->nitems_new = 0;

    foreach_item(cfd, curitem) {
        usetnw(cfd, curitem);
        icachebad(curitem);
    }

    if (cfd->nitems)
        cfd->nitems--;

    newitem = NULL;
    ptr = NULL;
    items_parsed = 0;

    XML_Parser p = XML_ParserCreate("UTF-8");

    if (!p)
        return;

    filename = concat(nc->feed_dir, curfeed->handle);

    XML_SetElementHandler(p, start, end);
    XML_SetCharacterDataHandler(p, strings);

    if ((fp = fopen(filename, "r"))) {
        while (!feof(fp)) {
            len = fread(buff, 1, BUFFER, fp);

            if (!(error = XML_Parse(p, buff, len, feof(fp)))) {
                logit("Parser Error: %s (%d)\n", XML_ErrorString(error),
                      XML_GetCurrentLineNumber(p));
                break;
            }
        }
        fclose(fp);
    }

    if (newitem) {
        free(newitem->title);
        free(newitem->link);
        free(newitem);
        free(descr);
    }

    if (cfd->nitems)
        cfd->nitems++;

    XML_ParserFree(p);
    free(buff);
    free(filename);
}
