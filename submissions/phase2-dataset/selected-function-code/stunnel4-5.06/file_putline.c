    int len;
    char *buff;
#ifdef USE_WIN32
    DWORD num;
#else /* USE_WIN32 */
    int num;
#endif /* USE_WIN32 */

    len=strlen(line);
    buff=str_alloc(len+2); /* +2 for CR+LF */
    strcpy(buff, line);
#ifdef USE_WIN32
    buff[len++]='\r'; /* CR */
#endif /* USE_WIN32 */
    buff[len++]='\n'; /* LF */
#ifdef USE_WIN32
    WriteFile(df->fh, buff, len, &num, NULL);
#else /* USE_WIN32 */
    /* no file -> write to stderr */
    num=write(df ? df->fd : 2, buff, len);
#endif /* USE_WIN32 */
    str_free(buff);
    return num;
}
