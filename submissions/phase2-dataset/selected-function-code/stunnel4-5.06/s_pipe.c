int s_pipe(int pipefd[2], int nonblock, char *msg) {
    int retval;

#ifdef USE_NEW_LINUX_API
    if(nonblock)
        retval=pipe2(pipefd, O_NONBLOCK|O_CLOEXEC);
    else
        retval=pipe2(pipefd, O_CLOEXEC);
#else
    retval=pipe(pipefd);
#endif
    if(retval<0) {
        ioerror(msg);
        return -1;
    }
    if(setup_fd(pipefd[0], nonblock, msg)<0) {
        close(pipefd[1]);
        return -1;
    }
    if(setup_fd(pipefd[1], nonblock, msg)<0) {
        close(pipefd[0]);
        return -1;
    }
    return 0;
}
