/* NRSS - ncurses RSS reader
   Copyright (C) 2007 Jack Miller <jjm2n4@umr.edu>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 as 
   published by the Free Software Foundation.
*/
#include "nrss.h"

#define WGET_ESCAPES 3
static struct escape wget_escapes[WGET_ESCAPES] = {
    {"%u", NULL},
    {"%p", NULL},
    {"%a", "NRSS-1.0.0"}
};

#define BROWSER_ESCAPES 1
static struct escape browser_escapes[BROWSER_ESCAPES] = {
    {"%u", NULL}
};

char **make_args(char *format)
{
    char **ret = NULL;
    int i = 0;
    int toks = 2;

    char *tok = NULL;
    char *save = NULL;

    int quote = 0;

    for (i = 0; format[i] != 0; i++) {
        if ((format[i] == ' ')&&(!quote))
            toks++;
        else if (format[i] == '"')
            quote = !quote;
    }

    ret = xmalloc(sizeof(char *) * toks);

    tok = strtok_r(format, " ", &save);
    ret[0] = tok;

    for (i = 1; i < (toks - 1); i++) {
        if (save[0] == '\"') {
            save++;
            ret[i] = strtok_r(NULL, "\"", &save);
        } else
            ret[i] = strtok_r(NULL, " ", &save);
    }

    ret[i] = NULL;
    return ret;
}

pid_t wget(char *URL, char *fname)
{
    char **args = NULL;
    char *escaped_string = NULL;
    pid_t pid = 0;

    wget_escapes[0].replacement = URL;
    wget_escapes[1].replacement = concat(nc->feed_dir, fname);

    escaped_string =
        escape_string(nc->wget_line, wget_escapes, WGET_ESCAPES);
    args = make_args(escaped_string);

    setpgid(getpid(), getpid());

    if (!(pid = fork())) {
        setpgid(getpid(), getpid());
        execve(args[0], args, NULL);
        exit(-1);
    }

    free(args);
    free(wget_escapes[1].replacement);
    wget_escapes[1].replacement = NULL;
    free(escaped_string);

    return pid;
}

void open_url(char *URL)
{
    char **args = NULL;
    char *escaped_string = NULL;
    pid_t pid = 0;

    if (!nc->browser_line) {
        status_message("No browser defined.");
        return;
    }

    browser_escapes[0].replacement = URL;
    escaped_string =
        escape_string(nc->browser_line, browser_escapes, BROWSER_ESCAPES);
    args = make_args(escaped_string);

    setpgid(getpid(), getpid());

    if (!(pid = fork())) {
        setpgid(getpid(), getpid());
        close(STDERR_FILENO);
        execve(args[0], args, environ);
        exit(-1);
    }

    if (browser_wait()) {
        iface_mute();
        tcsetpgrp(STDIN_FILENO, pid);
        waitpid(pid, NULL, 0);
        tcsetpgrp(STDIN_FILENO, getpid());
        iface_unmute();
        wnch(0);
    }

    free(args);
    free(escaped_string);
}

char *filter_string(char *str, char *cmd)
{
    char **args = NULL;
    char *filtered_str = NULL;
    char *arg_str = xstrdup(cmd);

    int inpipe[2];
    int outpipe[2];

    pid_t pid = 0;
    pipe(inpipe);
    pipe(outpipe);

    args = make_args(arg_str);

    setpgid(getpid(), getpid());

    if (!(pid = fork())) {
        dup2(inpipe[0], STDIN_FILENO);
        close(inpipe[1]);

        dup2(outpipe[1], STDOUT_FILENO);
        close(outpipe[0]);

        close(STDERR_FILENO);

        setpgid(getpid(), getpid());
        execve(args[0], args, NULL);
        exit(-1);
    }

    close(inpipe[0]);
    close(outpipe[1]);

    write_string_to_fd(str, inpipe[1]);
    close(inpipe[1]);

    filtered_str = read_string_from_fd(outpipe[0]);
    close(outpipe[0]);

    free(args);
    free(arg_str);
    return filtered_str;
}
