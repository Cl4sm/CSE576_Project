NOEXPORT char *connect_client(CLI *c, SERVICE_OPTIONS *opt, const PHASE phase) {
    char *line, *encoded;

    if(phase!=PROTOCOL_MIDDLE)
        return NULL;
    if(!opt->protocol_host) {
        s_log(LOG_ERR, "protocolHost not specified");
        longjmp(c->err, 1);
    }
    fd_printf(c, c->remote_fd.fd, "CONNECT %s HTTP/1.1",
        opt->protocol_host);
    fd_printf(c, c->remote_fd.fd, "Host: %s", opt->protocol_host);
    if(opt->protocol_username && opt->protocol_password) {
        if(!strcasecmp(opt->protocol_authentication, "ntlm")) {
#if !defined(OPENSSL_NO_MD4) && OPENSSL_VERSION_NUMBER>=0x0090700fL
            ntlm(c, opt);
#else
            s_log(LOG_ERR, "NTLM authentication is not available");
            longjmp(c->err, 1);
#endif
        } else { /* basic authentication */
            line=str_printf("%s:%s",
                opt->protocol_username, opt->protocol_password);
            encoded=base64(1, line, strlen(line));
            str_free(line);
            if(!encoded) {
                s_log(LOG_ERR, "Base64 encoder failed");
                longjmp(c->err, 1);
            }
            fd_printf(c, c->remote_fd.fd, "Proxy-Authorization: basic %s",
                encoded);
            str_free(encoded);
        }
    }
    fd_putline(c, c->remote_fd.fd, ""); /* empty line */
    line=fd_getline(c, c->remote_fd.fd);
    if(!is_prefix(line, "HTTP/1.0 2") && !is_prefix(line, "HTTP/1.1 2")) {
        /* not "HTTP/1.x 2xx Connection established" */
        s_log(LOG_ERR, "CONNECT request rejected");
        do { /* read all headers */
            str_free(line);
            line=fd_getline(c, c->remote_fd.fd);
        } while(*line);
        str_free(line);
        longjmp(c->err, 1);
    }
    s_log(LOG_INFO, "CONNECT request accepted");
    do {
        str_free(line);
        line=fd_getline(c, c->remote_fd.fd); /* read all headers */
    } while(*line);
    str_free(line);
    return NULL;
}
