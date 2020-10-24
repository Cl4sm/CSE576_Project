static FILE *
openLogFile(void)
{
    int fd;
    FILE *f;

    fd = open(logFile->string, O_WRONLY | O_CREAT | O_APPEND,
              logFilePermissions);
    if(fd < 0)
        return NULL;

    f = fdopen(fd, "a");
    if(f == NULL) {
        int saved_errno = errno;
        close(fd);
        errno = saved_errno;
        return NULL;
    }

    setvbuf(f, NULL, _IOLBF, 0);
    return f;
}