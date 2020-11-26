/* NRSS - ncurses RSS reader
   Copyright (C) 2007 Jack Miller <jjm2n4@umr.edu>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 as 
   published by the Free Software Foundation.
*/
#include "nrss.h"

#ifdef BSD
char *strndup(const char *line, int len)
{
    char *buf = xmalloc(len + 1);
    int i = 0;

    for (i = 0; i < len; i++) {
        if (line[i] == 0)
            break;
        buf[i] = line[i];
    }

    buf[i] = 0;
    return buf;
}
#endif

char *xstrdup(const char *s)
{
    char *temp;
    if ((temp = strdup(s)) == NULL) {
        logit("xstrdup: No memory! Bailing.\n");
        cleanup();
    }
    return temp;
}

char *xstrndup(const char *s, size_t n)
{
    char *temp;
    if ((temp = strndup(s, n)) == NULL) {
        logit("xstrndup: No memory! Bailing.\n");
        cleanup();
    }
    return temp;
}

void *xmalloc(size_t n)
{
    void *temp;
    if ((temp = malloc(n)) == NULL) {
        logit("xmalloc: No memory! Bailing.\n");
        cleanup();
    }
    return temp;
}

char *concat(char *one, char *two)
{
    char *line = NULL;

    line = xmalloc(strlen(one) + strlen(two) + 1);
    line = strcpy(line, one);
    line = strcat(line, two);

    return line;
}

int paren_type(char *str, int idx)
{
    if ((idx) && (str[idx - 1] != '\\')) {
        if (str[idx] == '(')
            return 1;
        else if (str[idx] == ')')
            return -1;
    }

    return 0;
}

void get_tern_range(char *tern, int *idx, int *len, int *end, int status)
{
    int paren_count = 1;
    int i = 4;

    while (tern[i]) {
        if ((tern[i] == ':') && (paren_count == 1)
            && (tern[i - 1] != '\\')) {
            if (status) {
                *idx = 4;
                *len = i - 4;
            } else
                *idx = i + 1;
            break;
        }

        paren_count += paren_type(tern, i);
        i++;
    }

    while ((tern[i]) && (paren_count)) {
        paren_count += paren_type(tern, i);
        i++;
    }

    if (!status)
        *len = i - *idx - 1;
    *end = i;
}

char *get_ternary(char *line, int *idx, int *len, int *end,
                  struct ternary *terns, int num_terns)
{
    char *tern = line;
    int i = 0;

    while ((tern = strstr(tern, "%?")) != NULL) {
        if ((tern == line) || (tern[-1] != '\\')) {
            for (i = 0; i < num_terns; i++) {
                if (terns[i].ident == tern[2]) {
                    get_tern_range(tern, idx, len, end, terns[i].status);
                    break;
                }
            }
        }
        return tern;
    }
    return NULL;
}

char *ternary_string(char *format, struct ternary *terns, int num_terns)
{
    char *line = NULL;
    char *sub = NULL;

    int idx = 0;
    int len = 0;
    int end = 0;

    int temp = 0;

    if (!format)
        return NULL;

    line = xstrdup(format);
    while ((sub = get_ternary(line, &idx, &len, &end,
                              terns, num_terns)) != NULL) {
        temp = strlen(sub + end);
        memmove(sub, sub + idx, len);
        memmove(sub + len, sub + end, temp + 1);
    }

    sub = xmalloc(strlen(line) + 1);
    strcpy(sub, line);
    free(line);
    return sub;
}

char *get_escape(char *line, int *idx, struct escape *escapes, int num_esc)
{
    char *esc = line;
    int i = 0;

    while ((esc = strstr(esc, "%")) != NULL) {
        if ((esc == line) || (esc[-1] != '\\')) {
            for (i = 0; i < num_esc; i++) {
                if (escapes[i].ident[1] == esc[1]) {
                    *idx = i;
                    return esc;
                }
            }
        }
        esc += 2;
    }
    return NULL;
}
char *escape_string(char *format, struct escape *escapes, int num_esc)
{
    char *line = NULL;
    char *sub = NULL;
    char *temp = NULL;

    int idx = 0;

    if (!format)
        return NULL;

    line = xstrdup(format);
    while ((sub = get_escape(line, &idx, escapes, num_esc)) != NULL) {
        if (!escapes[idx].replacement)
            escapes[idx].replacement = " ";

        temp =
            xmalloc(strlen(line) + strlen(escapes[idx].replacement) + 1);
        memset(temp, 0,
               strlen(line) + strlen(escapes[idx].replacement) + 1);
        temp = strncpy(temp, line, (sub - line));

        strcat(temp, escapes[idx].replacement);
        strcpy(strlen(temp) + temp, sub + 2);

        free(line);
        line = temp;
    }

    return line;
}
