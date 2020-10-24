int
specialRequestHandler(int status, 
                      FdEventHandlerPtr event, StreamRequestPtr srequest)
{
    SpecialRequestPtr request = srequest->data;
    int rc;
    int killed = 0;

    if(status < 0) {
        kill(request->pid, SIGTERM);
        killed = 1;
        request->object->flags &= ~OBJECT_INPROGRESS;
        abortObject(request->object, 502,
                    internAtomError(-status, "Couldn't read from client"));
        goto done;
    }

    if(srequest->offset > 0) {
        rc = objectAddData(request->object, request->buf, 
                           request->offset, srequest->offset);
        if(rc < 0) {
            kill(request->pid, SIGTERM);
            killed = 1;
            request->object->flags &= ~OBJECT_INPROGRESS;
            abortObject(request->object, 503,
                        internAtom("Couldn't add data to connection"));
            goto done;
        }
        request->offset += srequest->offset;
    }
    if(status) {
        request->object->flags &= ~OBJECT_INPROGRESS;
        request->object->length = request->object->size;
        goto done;
    }

    /* If we're the only person interested in this object, let's abort
       it now. */
    if(request->object->refcount <= 1) {
        kill(request->pid, SIGTERM);
        killed = 1;
        request->object->flags &= ~OBJECT_INPROGRESS;
        abortObject(request->object, 500, internAtom("Aborted"));
        goto done;
    }
    notifyObject(request->object);
    do_stream(IO_READ | IO_NOTNOW, request->fd, 0, request->buf, CHUNK_SIZE,
              specialRequestHandler, request);
    return 1;

 done:
    close(request->fd);
    dispose_chunk(request->buf);
    releaseNotifyObject(request->object);
    /* That's a blocking wait.  It shouldn't block for long, as we've
       either already killed the child, or else we got EOF from it. */
    do {
        rc = waitpid(request->pid, &status, 0);
    } while(rc < 0 && errno == EINTR);
    if(rc < 0) {
        do_log(L_ERROR, "Wait for %d: %d\n", (int)request->pid, errno);
    } else {
        int normal = 
            (WIFEXITED(status) && WEXITSTATUS(status) == 0) ||
            (killed && WIFSIGNALED(status) && WTERMSIG(status) == SIGTERM);
        char *reason =
            WIFEXITED(status) ? "with status" : 
            WIFSIGNALED(status) ? "on signal" :
            "with unknown status";
        int value =
            WIFEXITED(status) ? WEXITSTATUS(status) :
            WIFSIGNALED(status) ? WTERMSIG(status) :
            status;
        do_log(normal ? D_CHILD : L_ERROR, 
               "Child %d exited %s %d.\n",
               (int)request->pid, reason, value);
    }
    free(request);
    return 1;
}