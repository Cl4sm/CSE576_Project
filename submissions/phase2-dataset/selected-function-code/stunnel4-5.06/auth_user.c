#ifndef _WIN32_WCE
    struct servent *s_ent;    /* structure for getservbyname */
#endif
    SOCKADDR_UNION ident;     /* IDENT socket name */
    char *line, *type, *system, *user;

    if(!c->opt->username)
        return; /* -u option not specified */
#ifdef HAVE_STRUCT_SOCKADDR_UN
    if(c->peer_addr.sa.sa_family==AF_UNIX) {
        s_log(LOG_INFO, "IDENT not supported on Unix sockets");
        return;
    }
#endif
    c->fd=s_socket(c->peer_addr.sa.sa_family, SOCK_STREAM,
        0, 1, "socket (auth_user)");
    if(c->fd<0)
        longjmp(c->err, 1);
    memcpy(&ident, &c->peer_addr, c->peer_addr_len);
#ifndef _WIN32_WCE
    s_ent=getservbyname("auth", "tcp");
    if(s_ent) {
        ident.in.sin_port=s_ent->s_port;
    } else
#endif
    {
        s_log(LOG_WARNING, "Unknown service 'auth': using default 113");
        ident.in.sin_port=htons(113);
    }
    if(s_connect(c, &ident, addr_len(&ident)))
        longjmp(c->err, 1);
    s_log(LOG_DEBUG, "IDENT server connected");
    fd_printf(c, c->fd, "%u , %u",
        ntohs(c->peer_addr.in.sin_port),
        ntohs(c->opt->local_addr.in.sin_port));
    line=fd_getline(c, c->fd);
    closesocket(c->fd);
    c->fd=-1; /* avoid double close on cleanup */
    type=strchr(line, ':');
    if(!type) {
        s_log(LOG_ERR, "Malformed IDENT response");
        str_free(line);
        longjmp(c->err, 1);
    }
    *type++='\0';
    system=strchr(type, ':');
    if(!system) {
        s_log(LOG_ERR, "Malformed IDENT response");
        str_free(line);
        longjmp(c->err, 1);
    }
    *system++='\0';
    if(strcmp(type, " USERID ")) {
        s_log(LOG_ERR, "Incorrect INETD response type");
        str_free(line);
        longjmp(c->err, 1);
    }
    user=strchr(system, ':');
    if(!user) {
        s_log(LOG_ERR, "Malformed IDENT response");
        str_free(line);
        longjmp(c->err, 1);
    }
    *user++='\0';
    while(*user==' ') /* skip leading spaces */
        ++user;
    if(strcmp(user, c->opt->username)) {
        safestring(user);
        s_log(LOG_WARNING, "Connection from %s REFUSED by IDENT (user %s)",
            accepted_address, user);
        str_free(line);
        longjmp(c->err, 1);
    }
    s_log(LOG_INFO, "IDENT authentication passed");
    str_free(line);
}
