int make_sockets(int fd[2]) { /* make a pair of connected ipv4 sockets */
#ifdef INET_SOCKET_PAIR
    struct sockaddr_in addr;
    socklen_t addrlen;
    int s; /* temporary socket awaiting for connection */

    /* create two *blocking* sockets first */
    s=s_socket(AF_INET, SOCK_STREAM, 0, 0, "make_sockets: s_socket#1");
    if(s<0) {
        return 1;
    }
    fd[1]=s_socket(AF_INET, SOCK_STREAM, 0, 0, "make_sockets: s_socket#2");
    if(fd[1]<0) {
        closesocket(s);
        return 1;
    }

    addrlen=sizeof addr;
    memset(&addr, 0, addrlen);
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
    addr.sin_port=htons(0); /* dynamic port allocation */
    if(bind(s, (struct sockaddr *)&addr, addrlen))
        log_error(LOG_DEBUG, get_last_socket_error(), "make_sockets: bind#1");
    if(bind(fd[1], (struct sockaddr *)&addr, addrlen))
        log_error(LOG_DEBUG, get_last_socket_error(), "make_sockets: bind#2");

    if(listen(s, 1)) {
        sockerror("make_sockets: listen");
        closesocket(s);
        closesocket(fd[1]);
        return 1;
    }
    if(getsockname(s, (struct sockaddr *)&addr, &addrlen)) {
        sockerror("make_sockets: getsockname");
        closesocket(s);
        closesocket(fd[1]);
        return 1;
    }
    if(connect(fd[1], (struct sockaddr *)&addr, addrlen)) {
        sockerror("make_sockets: connect");
        closesocket(s);
        closesocket(fd[1]);
        return 1;
    }
    fd[0]=s_accept(s, (struct sockaddr *)&addr, &addrlen, 1,
        "make_sockets: s_accept");
    if(fd[0]<0) {
        closesocket(s);
        closesocket(fd[1]);
        return 1;
    }
    closesocket(s); /* don't care about the result */
    set_nonblock(fd[0], 1);
    set_nonblock(fd[1], 1);
#else
    if(s_socketpair(AF_UNIX, SOCK_STREAM, 0, fd, 1, "make_sockets: socketpair"))
        return 1;
#endif
    return 0;
}
