static void clean_env(void)
{
    char pathbuf[512];
    char **cleanenv;
    char **ep;
    int cidx = 0;
    int idx;

    /* While cleaning the environment, the environment should be clean.
     * (e.g. malloc() may get the name of a file for writing debugging info.
     * Bad news if MALLOC_DEBUG_FILE is set to /etc/passwd.  Sprintf() may be
     * susceptible to bad locale settings....)
     * (from PR 2790)
     */
    char **envp = environ;
    char *empty_ptr = NULL;

    environ = &empty_ptr; /* VERY safe environment */

    if ((cleanenv = (char **) calloc(AP_ENVBUF, sizeof(char *))) == NULL) {
        log_err("failed to malloc memory for environment\n");
        exit(123);
    }

    sprintf(pathbuf, "PATH=%s", AP_SAFE_PATH);
    cleanenv[cidx] = strdup(pathbuf);
    if (cleanenv[cidx] == NULL) {
        log_err("failed to malloc memory for environment\n");
        exit(124);
    }
    cidx++;

    for (ep = envp; *ep && cidx < AP_ENVBUF-1; ep++) {
        for (idx = 0; safe_env_lst[idx]; idx++) {
            if (!strncmp(*ep, safe_env_lst[idx],
                         strlen(safe_env_lst[idx]))) {
                cleanenv[cidx] = *ep;
                cidx++;
                break;
            }
        }
    }

    cleanenv[cidx] = NULL;

    environ = cleanenv;
}