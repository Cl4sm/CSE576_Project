int main(int argc, char *argv[]) {
    static struct WSAData wsa_state;
    TCHAR *c, stunnel_exe_path[MAX_PATH];

    /* set current working directory and engine path */
    GetModuleFileName(0, stunnel_exe_path, MAX_PATH);
    c=_tcsrchr(stunnel_exe_path, TEXT('\\')); /* last backslash */
    if(c) /* found */
        c[1]=TEXT('\0'); /* truncate program name */
#ifndef _WIN32_WCE
    if(!SetCurrentDirectory(stunnel_exe_path)) {
        /* log to stderr, as s_log() is not initialized */
        _ftprintf(stderr, TEXT("Cannot set directory to %s"),
            stunnel_exe_path);
        return 1;
    }
#endif
    _tputenv_s(TEXT("OPENSSL_ENGINES"), stunnel_exe_path);

    str_init(); /* initialize per-thread string management */
    if(WSAStartup(MAKEWORD(1, 1), &wsa_state))
        return 1;
    resolver_init();
    main_init();
    if(!main_configure(argc>1 ? argv[1] : NULL, argc>2 ? argv[2] : NULL))
        daemon_loop();
    main_cleanup();
    return 0;
}
