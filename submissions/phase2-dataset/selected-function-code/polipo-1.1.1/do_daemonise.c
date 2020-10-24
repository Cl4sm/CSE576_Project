void
do_daemonise(int noclose)
{
    int rc;

    fflush(stdout);
    fflush(stderr);

    rc = fork();
    if(rc < 0) {
        do_log_error(L_ERROR, errno, "Couldn't fork");
        exit(1);
    }

    if(rc > 0)
        exit(0);

    if(!noclose) {
        int fd;
        close(0);
        close(1);
        close(2);
        /* Leaving the default file descriptors free is not a good
           idea, as it will cause library functions such as abort to
           thrash the on-disk cache. */
        fd = open("/dev/null", O_RDONLY);
        if(fd > 0) {
            dup2(fd, 0);
            close(fd);
        }
        fd = open("/dev/null", O_WRONLY);
        if(fd >= 0) {
            if(fd != 1)
                dup2(fd, 1);
            if(fd != 2)
                dup2(fd, 2);
            if(fd != 1 && fd != 2)
                close(fd);
        }
    }
    rc = setsid();
    if(rc < 0) {
        do_log_error(L_ERROR, errno, "Couldn't create new session");
        exit(1);
    }
}