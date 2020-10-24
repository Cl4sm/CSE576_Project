void
eventLoop()
{
    struct timeval sleep_time, timeout;
    int rc, i, done, n;
    FdEventHandlerPtr event;
    int fd0;

    gettimeofday(&current_time, NULL);

    while(1) {
    again:
        if(exitFlag) {
            if(exitFlag < 3)
                reopenLog();
            if(exitFlag >= 2) {
                discardObjects(1, 0);
                if(exitFlag >= 3)
                    return;
                free_chunk_arenas();
            } else {
                writeoutObjects(1);
            }
            initForbidden();
            exitFlag = 0;
        }

        timeToSleep(&sleep_time);
        if(sleep_time.tv_sec == -1) {
            rc = poll(poll_fds, fdEventNum, 
                      diskIsClean ? -1 : idleTime * 1000);
        } else if(timeval_cmp(&sleep_time, &current_time) <= 0) {
            runTimeEventQueue();
            continue;
        } else {
            gettimeofday(&current_time, NULL);
            if(timeval_cmp(&sleep_time, &current_time) <= 0) {
                runTimeEventQueue();
                continue;
            } else {
                int t;
                timeval_minus(&timeout, &sleep_time, &current_time);
                t = timeout.tv_sec * 1000 + (timeout.tv_usec + 999) / 1000;
                rc = poll(poll_fds, fdEventNum,
                          diskIsClean ? t : MIN(idleTime * 1000, t));
            }
        }

        gettimeofday(&current_time, NULL);

        if(rc < 0) {
            if(errno == EINTR) {
                continue;
            } else if(errno == ENOMEM) {
                free_chunk_arenas();
                do_log(L_ERROR, 
                       "Couldn't poll: out of memory.  "
                       "Sleeping for one second.\n");
                sleep(1);
            } else {
                do_log_error(L_ERROR, errno, "Couldn't poll");
                exitFlag = 3;
            }
            continue;
        }

        if(rc == 0) {
            if(!diskIsClean) {
                timeToSleep(&sleep_time);
                if(timeval_cmp(&sleep_time, &current_time) > 0)
                    writeoutObjects(0);
            }
            continue;
        }

        /* Rather than tracking all changes to the in-memory cache, we
           assume that something changed whenever we see any activity. */
        diskIsClean = 0;

        fd0 = 
            (current_time.tv_usec ^ (current_time.tv_usec >> 16)) % fdEventNum;
        n = rc;
        for(i = 0; i < fdEventNum; i++) {
            int j = (i + fd0) % fdEventNum;
            if(n <= 0)
                break;
            if(poll_fds[j].revents) {
                n--;
                event = findEvent(poll_fds[j].revents, fdEvents[j]);
                if(!event)
                    continue;
                done = event->handler(0, event);
                if(done) {
                    if(fds_invalid)
                        unregisterFdEvent(event);
                    else
                        unregisterFdEventI(event, j);
                }
                if(fds_invalid) {
                    fds_invalid = 0;
                    goto again;
                } 
            }
        }
    }
}