    char *name, host[40], port[6];
    int fd[2], pid;
    X509 *peer;
#ifdef HAVE_PTHREAD_SIGMASK
    sigset_t newmask;
#endif

    if(c->opt->option.pty) {
        char tty[64];

        if(pty_allocate(fd, fd+1, tty))
            longjmp(c->err, 1);
        s_log(LOG_DEBUG, "TTY=%s allocated", tty);
    } else
        if(make_sockets(fd))
            longjmp(c->err, 1);

    pid=fork();
    c->pid=(unsigned long)pid;
    switch(pid) {
    case -1:    /* error */
        closesocket(fd[0]);
        closesocket(fd[1]);
        ioerror("fork");
        longjmp(c->err, 1);
    case  0:    /* child */
        closesocket(fd[0]);
        set_nonblock(fd[1], 0); /* switch back to blocking mode */
        /* dup2() does not copy FD_CLOEXEC flag */
        dup2(fd[1], 0);
        dup2(fd[1], 1);
        if(!global_options.option.foreground)
            dup2(fd[1], 2);
        closesocket(fd[1]); /* not really needed due to FD_CLOEXEC */

        if(!getnameinfo(&c->peer_addr.sa, c->peer_addr_len,
                host, 40, port, 6, NI_NUMERICHOST|NI_NUMERICSERV)) {
            /* just don't set these variables if getnameinfo() fails */
            putenv(str_printf("REMOTE_HOST=%s", host));
            putenv(str_printf("REMOTE_PORT=%s", port));
            if(c->opt->option.transparent_src) {
#ifndef LIBDIR
#define LIBDIR "."
#endif
#ifdef MACH64
                putenv("LD_PRELOAD_32=" LIBDIR "/libstunnel.so");
                putenv("LD_PRELOAD_64=" LIBDIR "/" MACH64 "/libstunnel.so");
#elif __osf /* for Tru64 _RLD_LIST is used instead */
                putenv("_RLD_LIST=" LIBDIR "/libstunnel.so:DEFAULT");
#else
                putenv("LD_PRELOAD=" LIBDIR "/libstunnel.so");
#endif
            }
        }

        if(c->ssl) {
            peer=SSL_get_peer_certificate(c->ssl);
            if(peer) {
                name=X509_NAME2text(X509_get_subject_name(peer));
                putenv(str_printf("SSL_CLIENT_DN=%s", name));
                name=X509_NAME2text(X509_get_issuer_name(peer));
                putenv(str_printf("SSL_CLIENT_I_DN=%s", name));
                X509_free(peer);
            }
        }
#ifdef HAVE_PTHREAD_SIGMASK
        sigemptyset(&newmask);
        sigprocmask(SIG_SETMASK, &newmask, NULL);
#endif
        signal(SIGCHLD, SIG_DFL);
        signal(SIGHUP, SIG_DFL);
        signal(SIGUSR1, SIG_DFL);
        signal(SIGPIPE, SIG_DFL);
        signal(SIGTERM, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGINT, SIG_DFL);
        execvp(c->opt->execname, c->opt->execargs);
        ioerror(c->opt->execname); /* execvp failed */
        _exit(1);
    default: /* parent */
        s_log(LOG_INFO, "Local mode child started (PID=%lu)", c->pid);
        closesocket(fd[1]);
        return fd[0];
    }
}
