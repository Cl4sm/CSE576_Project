NOEXPORT int create_pid(void) {
    int pf;
    char *pid;

    if(!global_options.pidfile) {
        s_log(LOG_DEBUG, "No pid file being created");
        return 0;
    }
    if(global_options.pidfile[0]!='/') {
        /* to prevent creating pid file relative to '/' after daemonize() */
        s_log(LOG_ERR, "Pid file (%s) must be full path name", global_options.pidfile);
        return 1;
    }
    global_options.dpid=(unsigned long)getpid();

    /* silently remove old pid file */
    unlink(global_options.pidfile);
    pf=open(global_options.pidfile, O_WRONLY|O_CREAT|O_TRUNC|O_EXCL, 0644);
    if(pf==-1) {
        s_log(LOG_ERR, "Cannot create pid file %s", global_options.pidfile);
        ioerror("create");
        return 1;
    }
    pid=str_printf("%lu\n", global_options.dpid);
    if(write(pf, pid, strlen(pid))<(int)strlen(pid)) {
        s_log(LOG_ERR, "Cannot write pid file %s", global_options.pidfile);
        ioerror("write");
        return 1;
    }
    str_free(pid);
    close(pf);
    s_log(LOG_DEBUG, "Created pid file %s", global_options.pidfile);
    atexit(delete_pid);
    return 0;
}
