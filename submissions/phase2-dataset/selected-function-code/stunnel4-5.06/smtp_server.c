    char *line;

    (void)opt; /* skip warning about unused parameter */
    if(phase==PROTOCOL_CHECK)
        opt->option.connect_before_ssl=1; /* c->remote_fd needed */
    if(phase!=PROTOCOL_MIDDLE)
        return NULL;
    s_poll_init(c->fds);
    s_poll_add(c->fds, c->local_rfd.fd, 1, 0);
    switch(s_poll_wait(c->fds, 0, 200)) { /* wait up to 200ms */
    case 0: /* fd not ready to read */
        s_log(LOG_DEBUG, "RFC 2487 detected");
        break;
    case 1: /* fd ready to read */
        s_log(LOG_DEBUG, "RFC 2487 not detected");
        return NULL; /* return if RFC 2487 is not used */
    default: /* -1 */
        sockerror("RFC2487 (s_poll_wait)");
        longjmp(c->err, 1);
    }

    line=fd_getline(c, c->remote_fd.fd);
    if(!is_prefix(line, "220")) {
        s_log(LOG_ERR, "Unknown server welcome");
        str_free(line);
        longjmp(c->err, 1);
    }
    fd_printf(c, c->local_wfd.fd, "%s + stunnel", line);
    str_free(line);
    line=fd_getline(c, c->local_rfd.fd);
    if(!is_prefix(line, "EHLO ")) {
        s_log(LOG_ERR, "Unknown client EHLO");
        str_free(line);
        longjmp(c->err, 1);
    }
    fd_printf(c, c->local_wfd.fd, "250-%s Welcome", line);
    fd_putline(c, c->local_wfd.fd, "250 STARTTLS");
    str_free(line);
    line=fd_getline(c, c->local_rfd.fd);
    if(!is_prefix(line, "STARTTLS")) {
        s_log(LOG_ERR, "STARTTLS expected");
        str_free(line);
        longjmp(c->err, 1);
    }
    fd_putline(c, c->local_wfd.fd, "220 Go ahead");
    str_free(line);
    return NULL;
}
