static int
translateFacility(AtomPtr facility)
{
    typedef struct
    {
        const char *name;
        int facility;
    } FacilitiesRec;

    /* List of all known valid syslog facilities.

       This list is terminated by a NULL facility name. */

    FacilitiesRec facilities[] = {
        /* These are all the facilities found in glibc 2.5. */
#ifdef LOG_AUTH
        { "auth", LOG_AUTH },
#endif
#ifdef LOG_AUTHPRIV
        { "authpriv", LOG_AUTHPRIV },
#endif
#ifdef LOG_CRON
        { "cron", LOG_CRON },
#endif
#ifdef LOG_DAEMON
        { "daemon", LOG_DAEMON },
#endif
#ifdef LOG_FTP
        { "ftp", LOG_FTP },
#endif
#ifdef LOG_KERN
        { "kern", LOG_KERN },
#endif
#ifdef LOG_LPR
        { "lpr", LOG_LPR },
#endif
#ifdef LOG_MAIL
        { "mail", LOG_MAIL },
#endif
#ifdef LOG_NEWS
        { "news", LOG_NEWS },
#endif
#ifdef LOG_SYSLOG
        { "syslog", LOG_SYSLOG },
#endif
#ifdef LOG_uucp
        { "uucp", LOG_UUCP },
#endif
        /* These are required by POSIX. */
        { "user", LOG_USER },
        { "local0", LOG_LOCAL0 },
        { "local1", LOG_LOCAL1 },
        { "local2", LOG_LOCAL2 },
        { "local3", LOG_LOCAL3 },
        { "local4", LOG_LOCAL4 },
        { "local5", LOG_LOCAL5 },
        { "local6", LOG_LOCAL6 },
        { "local7", LOG_LOCAL7 },
        { NULL, 0 }};

    FacilitiesRec *current;

    /* It would be more fitting to return LOG_DAEMON, but POSIX does not
       guarantee the existence of that facility. */

    if(!facility) {
        return LOG_USER;
    }

    current = facilities;
    while(current->name) {
        if(!strcmp(current->name, atomString(facility))) {
            return current->facility;
        }
        current++;
    }

    /* This will go to stderr because syslog is not yet initialized. */
    do_log(L_ERROR, "Specified logFacility %s nonexistent on this system.",
           atomString(facility));

    return LOG_USER;
}