    char *line, *id, *tail, *capa;

    (void)opt; /* skip warning about unused parameter */
    if(phase==PROTOCOL_CHECK)
        opt->option.connect_before_ssl=1; /* c->remote_fd needed */
    if(phase!=PROTOCOL_MIDDLE)
        return NULL;
    s_poll_init(c->fds);
    s_poll_add(c->fds, c->local_rfd.fd, 1, 0);
    switch(s_poll_wait(c->fds, 0, 200)) {
    case 0: /* fd not ready to read */
        s_log(LOG_DEBUG, "RFC 2595 detected");
        break;
    case 1: /* fd ready to read */
        s_log(LOG_DEBUG, "RFC 2595 not detected");
        return NULL; /* return if RFC 2595 is not used */
    default: /* -1 */
        sockerror("RFC2595 (s_poll_wait)");
        longjmp(c->err, 1);
    }

    /* process server welcome and send it to client */
    line=fd_getline(c, c->remote_fd.fd);
    if(!is_prefix(line, "* OK")) {
        s_log(LOG_ERR, "Unknown server welcome");
        str_free(line);
        longjmp(c->err, 1);
    }
    capa=strstr(line, "CAPABILITY");
    if(!capa)
        capa=strstr(line, "capability");
    if(capa)
        *capa='K'; /* disable CAPABILITY within greeting */
    fd_printf(c, c->local_wfd.fd, "%s (stunnel)", line);

    id=str_dup("");
    while(1) { /* process client commands */
        str_free(line);
        line=fd_getline(c, c->local_rfd.fd);
        /* split line into id and tail */
        str_free(id);
        id=str_dup(line);
        tail=strchr(id, ' ');
        if(!tail)
            break;
        *tail++='\0';

        if(is_prefix(tail, "STARTTLS")) {
            fd_printf(c, c->local_wfd.fd,
                "%s OK Begin TLS negotiation now", id);
            str_free(line);
            str_free(id);
            return NULL; /* success */
        } else if(is_prefix(tail, "CAPABILITY")) {
            fd_putline(c, c->remote_fd.fd, line); /* send it to server */
            str_free(line);
            line=fd_getline(c, c->remote_fd.fd); /* get the capabilites */
            if(*line=='*') {
                /*
                 * append STARTTLS
                 * should also add LOGINDISABLED, but can't because
                 * of Mozilla bug #324138/#312009
                 * LOGIN would fail as "unexpected command", anyway
                 */
                fd_printf(c, c->local_wfd.fd, "%s STARTTLS", line);
                str_free(line);
                line=fd_getline(c, c->remote_fd.fd); /* next line */
            }
            fd_putline(c, c->local_wfd.fd, line); /* forward to the client */
            tail=strchr(line, ' ');
            if(!tail || !is_prefix(tail+1, "OK")) { /* not OK? */
                fd_putline(c, c->local_wfd.fd,
                    "* BYE unexpected server response");
                s_log(LOG_ERR, "Unexpected server response: %s", line);
                break;
            }
        } else if(is_prefix(tail, "LOGOUT")) {
            fd_putline(c, c->local_wfd.fd, "* BYE server terminating");
            fd_printf(c, c->local_wfd.fd, "%s OK LOGOUT completed", id);
            break;
        } else {
            fd_putline(c, c->local_wfd.fd, "* BYE stunnel: unexpected command");
            fd_printf(c, c->local_wfd.fd, "%s BAD %s unexpected", id, tail);
            s_log(LOG_ERR, "Unexpected client command %s", tail);
            break;
        }
    }
    /* clean server shutdown */
    str_free(id);
    fd_putline(c, c->remote_fd.fd, "stunnel LOGOUT");
    str_free(line);
    line=fd_getline(c, c->remote_fd.fd);
    if(*line=='*') {
        str_free(line);
        line=fd_getline(c, c->remote_fd.fd);
    }
    str_free(line);
    longjmp(c->err, 2); /* don't reset */
    return NULL; /* some C compilers require a return value */
}
