int file_getline(DISK_FILE *df, char *line, int len) {
    /* this version is really slow, but performance is not important here */
    /* (no buffering is implemented) */
    int i;
#ifdef USE_WIN32
    DWORD num;
#else /* USE_WIN32 */
    int num;
#endif /* USE_WIN32 */

    if(!df) /* not opened */
        return -1;

    for(i=0; i<len-1; i++) {
#ifdef USE_WIN32
        ReadFile(df->fh, line+i, 1, &num, NULL);
#else /* USE_WIN32 */
        num=read(df->fd, line+i, 1);
#endif /* USE_WIN32 */
        if(num!=1) { /* EOF */
            if(i) /* any previously retrieved data */
                break;
            else
                return -1;
        }
        if(line[i]=='\n') /* LF */
            break;
        if(line[i]=='\r') /* CR */
            --i; /* ignore - it must be the last check */
    }
    line[i]='\0';
    return i;
}
