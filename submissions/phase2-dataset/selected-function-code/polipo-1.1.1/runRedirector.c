int
runRedirector(pid_t *pid_return, int *read_fd_return, int *write_fd_return)
{
    int rc, rc2, status;
    pid_t pid;
    int filedes1[2], filedes2[2];
    sigset_t ss, old_mask;

    assert(redirector);

    if(redirector_buffer == NULL) {
        redirector_buffer = malloc(REDIRECTOR_BUFFER_SIZE);
        if(redirector_buffer == NULL)
            return -errno;
    }

    rc = pipe(filedes1);
    if(rc < 0) {
        rc = -errno;
        goto fail1;
    }


    rc = pipe(filedes2);
    if(rc < 0) {
        rc = -errno;
        goto fail2;
    }

    fflush(stdout);
    fflush(stderr);
    flushLog();

    interestingSignals(&ss);
    do {
        rc = sigprocmask(SIG_BLOCK, &ss, &old_mask);
    } while (rc < 0 && errno == EINTR);
    if(rc < 0) {
        rc = -errno;
        goto fail3;
    }

    pid = fork();
    if(pid < 0) {
        rc = -errno;
        goto fail4;
    }

    if(pid > 0) {
        do {
            rc = sigprocmask(SIG_SETMASK, &old_mask, NULL);
        } while(rc < 0 && errno == EINTR);

        if(rc < 0) {
            rc = -errno;
            goto fail4;
        }

        rc = setNonblocking(filedes1[1], 1);
        if(rc >= 0)
            rc = setNonblocking(filedes2[0], 1);
        if(rc < 0) {
            rc = -errno;
            goto fail4;
        }

        /* This is completely unnecesary -- if the redirector cannot be
           started, redirectorStreamHandler1 will get EPIPE straight away --,
           but it improves error messages somewhat. */
        rc = waitpid(pid, &status, WNOHANG);
        if(rc > 0) {
            logExitStatus(status);
            rc = -EREDIRECTOR;
            goto fail4;
        } else if(rc < 0) {
            rc = -errno;
            goto fail4;
        }

        *read_fd_return = filedes2[0];
        *write_fd_return = filedes1[1];

        *pid_return = pid;
        /* This comes at the end so that the fail* labels can work */
        close(filedes1[0]);
        close(filedes2[1]);
    } else {
        close(filedes1[1]);
        close(filedes2[0]);
        uninitEvents();
        do {
            rc = sigprocmask(SIG_SETMASK, &old_mask, NULL);
        } while (rc < 0 && errno == EINTR);
        if(rc < 0)
            exit(142);

        if(filedes1[0] != 0)
            dup2(filedes1[0], 0);
        if(filedes2[1] != 1)
            dup2(filedes2[1], 1);

        execlp(redirector->string, redirector->string, NULL);
        exit(142);
        /* NOTREACHED */
    }
    return 1;

 fail4:
    do {
        rc2 = sigprocmask(SIG_SETMASK, &old_mask, NULL);
    } while(rc2 < 0 && errno == EINTR);
 fail3:
    close(filedes2[0]);
    close(filedes2[1]);
 fail2:
    close(filedes1[0]);
    close(filedes1[1]);
 fail1:
    free(redirector_buffer);
    redirector_buffer = NULL;
    return rc;
}