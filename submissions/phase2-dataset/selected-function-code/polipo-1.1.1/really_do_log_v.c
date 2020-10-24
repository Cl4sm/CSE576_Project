void
really_do_log_v(int type, const char *f, va_list args)
{
    va_list args_copy;

    if(type & LOGGING_MAX & logLevel) {
        if(logF)
        {
            va_copy(args_copy, args);
            vfprintf(logF, f, args_copy);
            va_end(args_copy);
        }
#ifdef HAVE_SYSLOG
        if(logSyslog) {
            va_copy(args_copy, args);
            accumulateSyslogV(type, f, args_copy);
            va_end(args_copy);
        }
#endif
    }
}