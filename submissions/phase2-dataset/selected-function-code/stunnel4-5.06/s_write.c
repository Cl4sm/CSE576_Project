        /* simulate a blocking write */
    u8 *ptr=(u8 *)buf;
    int num;

    while(len>0) {
        s_poll_init(c->fds);
        s_poll_add(c->fds, fd, 0, 1); /* write */
        switch(s_poll_wait(c->fds, c->opt->timeout_busy, 0)) {
        case -1:
            sockerror("s_write: s_poll_wait");
            longjmp(c->err, 1); /* error */
        case 0:
            s_log(LOG_INFO, "s_write: s_poll_wait:"
                " TIMEOUTbusy exceeded: sending reset");
            longjmp(c->err, 1); /* timeout */
        case 1:
            break; /* OK */
        default:
            s_log(LOG_ERR, "s_write: s_poll_wait: unknown result");
            longjmp(c->err, 1); /* error */
        }
        num=writesocket(fd, (void *)ptr, len);
        switch(num) {
        case -1: /* error */
            sockerror("writesocket (s_write)");
            longjmp(c->err, 1);
        }
        ptr+=num;
        len-=num;
    }
}
