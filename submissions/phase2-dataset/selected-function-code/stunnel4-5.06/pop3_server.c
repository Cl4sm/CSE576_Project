    char *line;

    (void)opt; /* skip warning about unused parameter */
    if(phase==PROTOCOL_CHECK)
        opt->option.connect_before_ssl=1; /* c->remote_fd needed */
    if(phase!=PROTOCOL_MIDDLE)
        return NULL;
    line=fd_getline(c, c->remote_fd.fd);
    fd_printf(c, c->local_wfd.fd, "%s + stunnel", line);
    str_free(line);
    line=fd_getline(c, c->local_rfd.fd);
    if(is_prefix(line, "CAPA")) { /* client wants RFC 2449 extensions */
        fd_putline(c, c->local_wfd.fd, "+OK Stunnel capability list follows");
        fd_putline(c, c->local_wfd.fd, "STLS");
        fd_putline(c, c->local_wfd.fd, ".");
        str_free(line);
        line=fd_getline(c, c->local_rfd.fd);
    }
    if(!is_prefix(line, "STLS")) {
        s_log(LOG_ERR, "Client does not want TLS");
        str_free(line);
        longjmp(c->err, 1);
    }
    str_free(line);
    fd_putline(c, c->local_wfd.fd, "+OK Stunnel starts TLS negotiation");
    return NULL;
}
