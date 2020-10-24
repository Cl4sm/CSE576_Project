void
redirectorKill(void)
{
    int rc, status, dead;

    if(redirector_read_fd >= 0) {
        rc = waitpid(redirector_pid, &status, WNOHANG);
        dead = (rc > 0);
        close(redirector_read_fd);
        redirector_read_fd = -1;
        close(redirector_write_fd);
        redirector_write_fd = -1;
        if(!dead) {
            rc = kill(redirector_pid, SIGTERM);
            if(rc < 0 && errno != ESRCH) {
                do_log_error(L_ERROR, errno, "Couldn't kill redirector");
                redirector_pid = -1;
                return;
            }
            do {
                rc = waitpid(redirector_pid, &status, 0);
            } while(rc < 0 && errno == EINTR);
            if(rc < 0)
                do_log_error(L_ERROR, errno,
                             "Couldn't wait for redirector's death");
        } else
            logExitStatus(status);
        redirector_pid = -1;
    }
}