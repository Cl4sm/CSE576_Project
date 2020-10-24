    char *line;

    (void)opt; /* skip warning about unused parameter */
    if(phase!=PROTOCOL_MIDDLE)
        return NULL;
    line=fd_getline(c, c->remote_fd.fd);
    if(!is_prefix(line, "+OK ")) {
        s_log(LOG_ERR, "Unknown server welcome");
        str_free(line);
        longjmp(c->err, 1);
    }
    fd_putline(c, c->local_wfd.fd, line);
    fd_putline(c, c->remote_fd.fd, "STLS");
    str_free(line);
    line=fd_getline(c, c->remote_fd.fd);
    if(!is_prefix(line, "+OK ")) {
        s_log(LOG_ERR, "Server does not support TLS");
        str_free(line);
        longjmp(c->err, 1);
    }
    str_free(line);
    return NULL;
}
