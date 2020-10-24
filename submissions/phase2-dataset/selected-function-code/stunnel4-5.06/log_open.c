    if(global_options.output_file) { /* 'output' option specified */
        outfile=file_open(global_options.output_file,
            global_options.log_file_mode);
#if defined(USE_WIN32) && !defined(_WIN32_WCE)
        if(!outfile) {
            char appdata[MAX_PATH], *path;
            if(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA|CSIDL_FLAG_CREATE,
                    NULL, 0, appdata)==S_OK) {
                path=str_printf("%s\\%s", appdata, global_options.output_file);
                outfile=file_open(path, global_options.log_file_mode);
                if(outfile)
                    s_log(LOG_NOTICE, "Logging to %s", path);
                str_free(path);
            }
        }
#endif
        if(!outfile) {
            s_log(LOG_ERR, "Cannot open log file: %s",
                global_options.output_file);
            return 1;
        }
    }
    log_flush(LOG_MODE_CONFIGURED);
    return 0;
}
