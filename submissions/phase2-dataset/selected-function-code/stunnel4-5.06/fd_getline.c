    char *line, *tmpline;
    int ptr=0, allocated=32;

    line=str_alloc(allocated);
    for(;;) {
        s_poll_init(c->fds);
        s_poll_add(c->fds, fd, 1, 0); /* read */
        switch(s_poll_wait(c->fds, c->opt->timeout_busy, 0)) {
        case -1:
            sockerror("fd_getline: s_poll_wait");
            str_free(line);
            longjmp(c->err, 1); /* error */
        case 0:
            s_log(LOG_INFO, "fd_getline: s_poll_wait:"
                " TIMEOUTbusy exceeded: sending reset");
            str_free(line);
            longjmp(c->err, 1); /* timeout */
        case 1:
            break; /* OK */
        default:
            s_log(LOG_ERR, "fd_getline: s_poll_wait: Unknown result");
            str_free(line);
            longjmp(c->err, 1); /* error */
        }
        if(allocated<ptr+1) {
            allocated*=2;
            line=str_realloc(line, allocated);
        }
        switch(readsocket(fd, line+ptr, 1)) {
        case -1: /* error */
            sockerror("fd_getline: readsocket");
            str_free(line);
            longjmp(c->err, 1);
        case 0: /* EOF */
            s_log(LOG_ERR, "fd_getline: Unexpected socket close");
            str_free(line);
            longjmp(c->err, 1);
        }
        if(line[ptr]=='\r')
            continue;
        if(line[ptr]=='\n')
            break;
        if(line[ptr]=='\0')
            break;
        if(++ptr>65536) { /* >64KB --> DoS protection */
            s_log(LOG_ERR, "fd_getline: Line too long");
            str_free(line);
            longjmp(c->err, 1);
        }
    }
    line[ptr]='\0';
    tmpline=str_dup(line);
    safestring(tmpline);
    s_log(LOG_DEBUG, " <- %s", tmpline);
    str_free(tmpline);
    return line;
}
