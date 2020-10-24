static int my_system(char *cmd, char *opt) {
    int           pid;
    extern char **environ;

    if (cmd == NULL) return 1;
    pid = fork();
    if (pid == -1) return -1;
    if (pid == 0) {
        pid = fork();
        if (pid == 0) {
            char *argv[3];
            char *thiscommand = xmalloc(strlen(cmd)
                    + (opt ? strlen(opt) + 4 : 1));
            sprintf(thiscommand, "%s %s%s%s", cmd,
                    opt ? "\"" : "",
                    opt ? opt  : "",
                    opt ? "\"" : "");
            argv[0] = "sh";
            argv[1] = "-c";
            argv[2] = thiscommand;
            argv[3] = 0;
            execve("/bin/sh", argv, environ);
            FREE(thiscommand);
            exit(0);
        }
        exit(0);
    }
    return 0;
}