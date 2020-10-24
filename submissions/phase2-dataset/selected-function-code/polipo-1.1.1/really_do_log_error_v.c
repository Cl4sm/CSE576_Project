void
really_do_log_error_v(int type, int e, const char *f, va_list args)
{
    va_list args_copy;

    if((type & LOGGING_MAX & logLevel) != 0) {
        char *es = pstrerror(e);
        if(es == NULL)
            es = "Unknown error";

        if(logF) {
            va_copy(args_copy, args);
            vfprintf(logF, f, args_copy);
            fprintf(logF, ": %s\n", es);
            va_end(args_copy);
        }
#ifdef HAVE_SYSLOG
        if(logSyslog) {
            char msg[256];
            int n = 0;

            va_copy(args_copy, args);
            n = snnvprintf(msg, n, 256, f, args_copy);
            va_end(args_copy);
            n = snnprintf(msg, n, 256, ": ");
            n = snnprint_n(msg, n, 256, es, strlen (es));
            n = snnprintf(msg, n, 256, "\n");
            /* Overflow? Vanishingly unlikely; truncate at 255. */
            if(n < 0 || n > 256) {
                n = 256;
                msg[255] = '\0';
            }
            else
                msg[n] = '\0';

            accumulateSyslogN(type, msg, n);
        }
#endif
    }
}