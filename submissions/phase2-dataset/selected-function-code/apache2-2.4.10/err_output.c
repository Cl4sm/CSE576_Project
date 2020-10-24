static void err_output(int is_error, const char *fmt, va_list ap)
{
#ifdef AP_LOG_EXEC
    time_t timevar;
    struct tm *lt;

    if (!log) {
#if defined(_LARGEFILE64_SOURCE) && HAVE_FOPEN64
        if ((log = fopen64(AP_LOG_EXEC, "a")) == NULL) {
#else
        if ((log = fopen(AP_LOG_EXEC, "a")) == NULL) {
#endif
            fprintf(stderr, "suexec failure: could not open log file\n");
            perror("fopen");
            exit(1);
        }
    }

    if (is_error) {
        fprintf(stderr, "suexec policy violation: see suexec log for more "
                        "details\n");
    }

    time(&timevar);
    lt = localtime(&timevar);

    fprintf(log, "[%d-%.2d-%.2d %.2d:%.2d:%.2d]: ",
            lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday,
            lt->tm_hour, lt->tm_min, lt->tm_sec);

    vfprintf(log, fmt, ap);

    fflush(log);
#endif /* AP_LOG_EXEC */
    return;
}