void
initLog(void)
{
    if(daemonise && logFile == NULL && !logSyslog)
        logFile = internAtom("/var/log/polipo/polipo.log");

    if(logFile != NULL && logFile->length > 0) {
        FILE *f;
        logFile = expandTilde(logFile);
        f = openLogFile();
        if(f == NULL) {
            do_log_error(L_ERROR, errno, "Couldn't open log file %s",
                         logFile->string);
            exit(1);
        }
        logF = f;
    }

    if(logSyslog) {
        initSyslog();

        if(logFile == NULL) {
            logF = NULL;
        }
    }
}