int main_configure(char *arg1, char *arg2) {
    if(options_cmdline(arg1, arg2))
        return 1;
    options_apply();
    str_canary_init(); /* needs prng initialization from options_cmdline */
#if !defined(USE_WIN32) && !defined(__vms)
    /* syslog_open() must be called before change_root()
     * to be able to access /dev/log socket */
    syslog_open();
#endif /* !defined(USE_WIN32) && !defined(__vms) */
    if(bind_ports())
        return 1;

#ifdef HAVE_CHROOT
    /* change_root() must be called before drop_privileges()
     * since chroot() needs root privileges */
    if(change_root())
        return 1;
#endif /* HAVE_CHROOT */

    if(drop_privileges(1))
        return 1;

    /* log_open() must be be called after drop_privileges()
     * or logfile rotation won't be possible */
    /* log_open() must be be called before daemonize()
     * since daemonize() invalidates stderr */
    if(log_open())
        return 1;
#ifndef USE_FORK
    num_clients=0; /* the first valid config */
#endif
    return 0;
}
