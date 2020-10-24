    static char cache[PEM_BUFSIZE];
    int len;

    if(size>PEM_BUFSIZE)
        size=PEM_BUFSIZE;

    if(userdata) { /* prompt the user */
#ifdef USE_WIN32
        len=passwd_cb(buf, size, rwflag, userdata);
#else
        /* PEM_def_callback is defined in OpenSSL 0.9.7 and later */
        len=PEM_def_callback(buf, size, rwflag, NULL);
#endif
        memcpy(cache, buf, size); /* save in cache */
        cache_initialized=1;
    } else { /* try the cached value */
        strncpy(buf, cache, size);
        buf[size-1]='\0';
        len=strlen(buf);
    }
    return len;
}
