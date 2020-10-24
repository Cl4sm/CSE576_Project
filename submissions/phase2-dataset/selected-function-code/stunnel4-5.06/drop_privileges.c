#if defined(USE_WIN32) || defined(__vms) || defined(USE_OS2)
    (void)critical; /* skip warning about unused parameter */
#else
#ifdef HAVE_SETGROUPS
    gid_t gr_list[1];
#endif

    /* set uid and gid */
    if(global_options.gid) {
        if(setgid(global_options.gid) && critical) {
            sockerror("setgid");
            return 1;
        }
#ifdef HAVE_SETGROUPS
        gr_list[0]=global_options.gid;
        if(setgroups(1, gr_list) && critical) {
            sockerror("setgroups");
            return 1;
        }
#endif
    }
    if(global_options.uid) {
        if(setuid(global_options.uid) && critical) {
            sockerror("setuid");
            return 1;
        }
    }
#endif /* standard Unix */
    return 0;
}
