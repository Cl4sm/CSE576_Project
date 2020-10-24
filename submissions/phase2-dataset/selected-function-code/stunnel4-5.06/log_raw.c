        const char *id, const char *text) {
    char *line;

    /* build the line and log it to syslog/file */
    if(mode==LOG_MODE_CONFIGURED) { /* configured */
        line=str_printf("%s %s: %s", stamp, id, text);
        if(level<=global_options.debug_level) {
#if !defined(USE_WIN32) && !defined(__vms)
            if(global_options.option.syslog)
                syslog(level, "%s: %s", id, text);
#endif /* USE_WIN32, __vms */
            if(outfile)
                file_putline(outfile, line); /* send log to file */
        }
    } else if(mode==LOG_MODE_ERROR) {
        if(level>=0 || level<=7) /* just in case */
            line=str_printf("[%c] %s", "***!:.  "[level], text);
        else
            line=str_printf("[?] %s", text);
    } else /* LOG_MODE_INFO */
        line=str_dup(text); /* don't log the time stamp in error mode */

    /* log the line to the UI (GUI, stderr, etc.) */
    if(mode==LOG_MODE_ERROR ||
            (mode==LOG_MODE_INFO && level<LOG_DEBUG) ||
#if defined(USE_WIN32) || defined(USE_JNI)
            level<=global_options.debug_level
#else
            (level<=global_options.debug_level &&
            global_options.option.foreground)
#endif
            )
        ui_new_log(line);

    str_free(line);
}
