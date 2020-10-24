NOEXPORT int parse_debug_level(char *arg) {
    char *arg_copy;
    char *string;
    facilitylevel *fl;

/* facilities only make sense on unix */
#if !defined (USE_WIN32) && !defined (__vms)
    facilitylevel facilities[] = {
        {"auth", LOG_AUTH},     {"cron", LOG_CRON},     {"daemon", LOG_DAEMON},
        {"kern", LOG_KERN},     {"lpr", LOG_LPR},       {"mail", LOG_MAIL},
        {"news", LOG_NEWS},     {"syslog", LOG_SYSLOG}, {"user", LOG_USER},
        {"uucp", LOG_UUCP},     {"local0", LOG_LOCAL0}, {"local1", LOG_LOCAL1},
        {"local2", LOG_LOCAL2}, {"local3", LOG_LOCAL3}, {"local4", LOG_LOCAL4},
        {"local5", LOG_LOCAL5}, {"local6", LOG_LOCAL6}, {"local7", LOG_LOCAL7},

        /* some that are not on all unicies */
#ifdef LOG_AUTHPRIV
        {"authpriv", LOG_AUTHPRIV},
#endif
#ifdef LOG_FTP
        {"ftp", LOG_FTP},
#endif
#ifdef LOG_NTP
        {"ntp", LOG_NTP},
#endif
        {NULL, 0}
    };
#endif /* USE_WIN32, __vms */

    facilitylevel levels[] = {
        {"emerg", LOG_EMERG},     {"alert", LOG_ALERT},
        {"crit", LOG_CRIT},       {"err", LOG_ERR},
        {"warning", LOG_WARNING}, {"notice", LOG_NOTICE},
        {"info", LOG_INFO},       {"debug", LOG_DEBUG},
        {NULL, -1}
    };

    arg_copy=str_dup(arg);
    string=arg_copy;

/* facilities only make sense on Unix */
#if !defined (USE_WIN32) && !defined (__vms)
    if(strchr(string, '.')) { /* we have a facility specified */
        new_global_options.facility=-1;
        string=strtok(arg_copy, "."); /* break it up */

        for(fl=facilities; fl->name; ++fl) {
            if(!strcasecmp(fl->name, string)) {
                new_global_options.facility=fl->value;
                break;
            }
        }
        if(new_global_options.facility==-1)
            return 1; /* FAILED */
        string=strtok(NULL, ".");    /* set to the remainder */
    }
#endif /* USE_WIN32, __vms */

    /* time to check the syslog level */
    if(string && strlen(string)==1 && *string>='0' && *string<='7') {
        new_global_options.debug_level=*string-'0';
        return 0; /* OK */
    }
    new_global_options.debug_level=8;    /* illegal level */
    for(fl=levels; fl->name; ++fl) {
        if(!strcasecmp(fl->name, string)) {
            new_global_options.debug_level=fl->value;
            break;
        }
    }
    if(new_global_options.debug_level==8)
        return 1; /* FAILED */
    return 0; /* OK */
}
