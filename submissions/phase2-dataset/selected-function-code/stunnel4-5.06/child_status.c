void child_status(void) { /* dead libwrap or 'exec' process detected */
    int pid, status;

#ifdef HAVE_WAIT_FOR_PID
    while((pid=wait_for_pid(-1, &status, WNOHANG))>0) {
#else
    if((pid=wait(&status))>0) {
#endif
#ifdef WIFSIGNALED
        if(WIFSIGNALED(status)) {
            s_log(LOG_INFO, "Child process %d terminated on signal %d",
                pid, WTERMSIG(status));
        } else {
            s_log(LOG_INFO, "Child process %d finished with code %d",
                pid, WEXITSTATUS(status));
        }
#else
        s_log(LOG_INFO, "Child process %d finished with status %d",
            pid, status);
#endif
    }
}
