static void
fillSpecialObject(ObjectPtr object, void (*fn)(FILE*, char*), void* closure)
{
    int rc;
    int filedes[2];
    pid_t pid;
    sigset_t ss, old_mask;

    if(object->flags & OBJECT_INPROGRESS)
        return;

    rc = pipe(filedes);
    if(rc < 0) {
        do_log_error(L_ERROR, errno, "Couldn't create pipe");
        abortObject(object, 503,
                    internAtomError(errno, "Couldn't create pipe"));
        return;
    }

    fflush(stdout);
    fflush(stderr);
    flushLog();

    /* Block signals that we handle specially until the child can
       disable the handlers. */
    interestingSignals(&ss);
    /* I'm a little confused.  POSIX doesn't allow EINTR here, but I
       think that both Linux and SVR4 do. */
    do {
        rc = sigprocmask(SIG_BLOCK, &ss, &old_mask);
    } while (rc < 0 && errno == EINTR);
    if(rc < 0) {
        do_log_error(L_ERROR, errno, "Sigprocmask failed");
        abortObject(object, 503, internAtomError(errno, "Sigprocmask failed"));
        close(filedes[0]);
        close(filedes[1]);
        return;
    }
    
    pid = fork();
    if(pid < 0) {
        do_log_error(L_ERROR, errno, "Couldn't fork");
        abortObject(object, 503, internAtomError(errno, "Couldn't fork"));
        close(filedes[0]);
        close(filedes[1]);
        do {
            rc = sigprocmask(SIG_SETMASK, &old_mask, NULL);
        } while (rc < 0 && errno == EINTR);
        if(rc < 0) {
            do_log_error(L_ERROR, errno, "Couldn't restore signal mask");
            polipoExit();
        }
        return;
    }

    if(pid > 0) {
        SpecialRequestPtr request;
        close(filedes[1]);
        do {
            rc = sigprocmask(SIG_SETMASK, &old_mask, NULL);
        } while (rc < 0 && errno == EINTR);
        if(rc < 0) {
            do_log_error(L_ERROR, errno, "Couldn't restore signal mask");
            polipoExit();
            return;
        }

        request = malloc(sizeof(SpecialRequestRec));
        if(request == NULL) {
            kill(pid, SIGTERM);
            close(filedes[0]);
            abortObject(object, 503,
                        internAtom("Couldn't allocate request\n"));
            notifyObject(object);
            return;
        } else {
            request->buf = get_chunk();
            if(request->buf == NULL) {
                kill(pid, SIGTERM);
                close(filedes[0]);
                free(request);
                abortObject(object, 503,
                            internAtom("Couldn't allocate request\n"));
                notifyObject(object);
                return;
            }
        }
        object->flags |= OBJECT_INPROGRESS;
        retainObject(object);
        request->object = object;
        request->fd = filedes[0];
        request->pid = pid;
        request->offset = 0;
        /* Under any sensible scheduler, the child will run before the
           parent.  So no need for IO_NOTNOW. */
        do_stream(IO_READ, filedes[0], 0, request->buf, CHUNK_SIZE,
                  specialRequestHandler, request);
    } else {
        /* child */
        close(filedes[0]);
        uninitEvents();
        do {
            rc = sigprocmask(SIG_SETMASK, &old_mask, NULL);
        } while (rc < 0 && errno == EINTR);
        if(rc < 0)
            exit(1);

        if(filedes[1] != 1)
            dup2(filedes[1], 1);

        (*fn)(stdout, closure);
        exit(0);
    }
}