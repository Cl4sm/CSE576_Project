void fatal_debug(char *txt, char *file, int line) {
    char msg[80];
#ifdef USE_WIN32
    DWORD num;
#ifdef UNICODE
    TCHAR tmsg[80];
#endif
#endif /* USE_WIN32 */

    snprintf(msg, sizeof msg, /* with newline */
        "INTERNAL ERROR: %s at %s, line %d\n", txt, file, line);

    if(outfile) {
#ifdef USE_WIN32
        WriteFile(outfile->fh, msg, strlen(msg), &num, NULL);
#else /* USE_WIN32 */
        /* no file -> write to stderr */
        /* no meaningful way here to handle the result */
        write(outfile ? outfile->fd : 2, msg, strlen(msg));
#endif /* USE_WIN32 */
    }

#ifndef USE_WIN32
    if(mode!=LOG_MODE_CONFIGURED || global_options.option.foreground) {
        fputs(msg, stderr);
        fflush(stderr);
    }
#endif /* !USE_WIN32 */

    snprintf(msg, sizeof msg, /* without newline */
        "INTERNAL ERROR: %s at %s, line %d", txt, file, line);

#if !defined(USE_WIN32) && !defined(__vms)
    if(global_options.option.syslog)
        syslog(LOG_CRIT, "%s", msg);
#endif /* USE_WIN32, __vms */

#ifdef USE_WIN32
#ifdef UNICODE
    if(MultiByteToWideChar(CP_UTF8, 0, msg, -1, tmsg, 80))
        message_box(tmsg, MB_ICONERROR);
#else
    message_box(msg, MB_ICONERROR);
#endif
#endif /* USE_WIN32 */

    abort();
}
