static void
accumulateSyslogV(int type, const char *f, va_list args)
{
    int rc;
    va_list args_copy;

 again:
    va_copy(args_copy, args);
    rc = vsnprintf(syslogBuf + syslogBufLength,
                   syslogBufSize - syslogBufLength,
                   f, args_copy);
    va_end(args_copy);

    if(rc < 0 || rc >= syslogBufSize - syslogBufLength) {
        rc = expandSyslog(rc);
        if(rc < 0)
            return;
        goto again;
    }

    syslogBufLength += rc;

    maybeFlushSyslog(type);
}