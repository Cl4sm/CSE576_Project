void s_read(CLI *c, int fd, void *ptr, int len) {
        /* simulate a blocking read */
    int num;

    while(len>0) {
        s_poll_init(c->fds);
        s_poll_add(c->fds, fd, 1, 0); /* read */
        switch(s_poll_wait(c->fds, c->opt->timeout_busy, 0)) {
        case -1:
            sockerror("s_read: s_poll_wait");
            longjmp(c->err, 1); /* error */
        case 0:
            s_log(LOG_INFO, "s_read: s_poll_wait:"
                " TIMEOUTbusy exceeded: sending reset");
            longjmp(c->err, 1); /* timeout */
        case 1:
            break; /* OK */
        default:
            s_log(LOG_ERR, "s_read: s_poll_wait: unknown result");
            longjmp(c->err, 1); /* error */
        }
        num=readsocket(fd, ptr, len);
        switch(num) {
        case -1: /* error */
            sockerror("readsocket (s_read)");
            longjmp(c->err, 1);
        case 0: /* EOF */
            s_log(LOG_ERR, "Unexpected socket close (s_read)");
            longjmp(c->err, 1);
        }
        ptr=(u8 *)ptr+num;
        len-=num;
    }
}
