        int nonblock, char *msg) {
#ifdef USE_NEW_LINUX_API
    if(nonblock)
        type|=SOCK_NONBLOCK;
    type|=SOCK_CLOEXEC;
#endif
    if(socketpair(domain, type, protocol, sv)<0) {
        ioerror(msg);
        return -1;
    }
    if(setup_fd(sv[0], nonblock, msg)<0) {
        closesocket(sv[1]);
        return -1;
    }
    if(setup_fd(sv[1], nonblock, msg)<0) {
        closesocket(sv[0]);
        return -1;
    }
    return 0;
}
