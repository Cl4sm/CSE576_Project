    DISK_FILE *df;
    int fd, flags;

    /* open file */
    switch(mode) {
    case FILE_MODE_READ:
        flags=O_RDONLY;
        break;
    case FILE_MODE_APPEND:
        flags=O_CREAT|O_WRONLY|O_APPEND;
        break;
    case FILE_MODE_OVERWRITE:
        flags=O_CREAT|O_WRONLY|O_TRUNC;
        break;
    default: /* invalid mode */
        return NULL;
    }
#ifdef O_NONBLOCK
    flags|=O_NONBLOCK;
#elif defined O_NDELAY
    flags|=O_NDELAY;
#endif
#ifdef O_CLOEXEC
    flags|=O_CLOEXEC;
#endif /* O_CLOEXEC */
    fd=open(name, flags, 0640);
    if(fd<0)
        return NULL;

    /* setup df structure */
    df=str_alloc(sizeof df);
    df->fd=fd;
    return df;
}
