void
writePid(char *pidfile)
{
    int fd, n, rc;
    char buf[16];

    fd = open(pidfile, O_WRONLY | O_CREAT | O_EXCL, 0666);
    if(fd < 0) {
        do_log_error(L_ERROR, errno, 
                     "Couldn't create pid file %s", pidfile);
        exit(1);
    }
    n = snprintf(buf, 16, "%ld\n", (long)getpid());
    if(n < 0 || n >= 16) {
        close(fd);
        unlink(pidfile);
        do_log(L_ERROR, "Couldn't format pid.\n");
        exit(1);
    }
    rc = write(fd, buf, n);
    if(rc != n) {
        close(fd);
        unlink(pidfile);
        do_log_error(L_ERROR, errno, "Couldn't write pid");
        exit(1);
    }

    close(fd);
    return;
}