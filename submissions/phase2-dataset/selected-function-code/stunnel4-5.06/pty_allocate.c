int pty_allocate(int *ptyfd, int *ttyfd, char *namebuf) {
#if defined(HAVE_OPENPTY) || defined(BSD4_4) && !defined(__INNOTEK_LIBC__)
    /* openpty(3) exists in OSF/1 and some other os'es */
    char buf[64];
    int i;

    i=openpty(ptyfd, ttyfd, buf, NULL, NULL);
    if(i<0) {
        ioerror("openpty");
        return -1;
    }
    strcpy(namebuf, buf); /* possible truncation */
    return 0;
#else /* HAVE_OPENPTY */
#ifdef HAVE__GETPTY
    /*
     * _getpty(3) exists in SGI Irix 4.x, 5.x & 6.x -- it generates more
     * pty's automagically when needed
     */
    char *slave;

    slave=_getpty(ptyfd, O_RDWR, 0622, 0);
    if(slave==NULL) {
        ioerror("_getpty");
        return -1;
    }
    strcpy(namebuf, slave);
    /* open the slave side */
    *ttyfd=open(namebuf, O_RDWR|O_NOCTTY);
    if(*ttyfd<0) {
        ioerror(namebuf);
        close(*ptyfd);
        return -1;
    }
    return 0;
#else /* HAVE__GETPTY */
#if defined(HAVE_DEV_PTMX)
    /*
     * this code is used e.g. on Solaris 2.x
     * note that Solaris 2.3 * also has bsd-style ptys, but they simply do not
     * work
     */
    int ptm; char *pts;

    ptm=open("/dev/ptmx", O_RDWR|O_NOCTTY);
    if(ptm<0) {
        ioerror("/dev/ptmx");
        return -1;
    }
    if(grantpt(ptm)<0) {
        ioerror("grantpt");
        /* return -1; */
        /* can you tell me why it doesn't work? */
    }
    if(unlockpt(ptm)<0) {
        ioerror("unlockpt");
        return -1;
    }
    pts=ptsname(ptm);
    if(pts==NULL)
        s_log(LOG_ERR, "Slave pty side name could not be obtained");
    strcpy(namebuf, pts);
    *ptyfd=ptm;

    /* open the slave side */
    *ttyfd=open(namebuf, O_RDWR|O_NOCTTY);
    if(*ttyfd<0) {
        ioerror(namebuf);
        close(*ptyfd);
        return -1;
    }
    /* push the appropriate streams modules, as described in Solaris pts(7) */
    if(ioctl(*ttyfd, I_PUSH, "ptem")<0)
        ioerror("ioctl I_PUSH ptem");
    if(ioctl(*ttyfd, I_PUSH, "ldterm")<0)
        ioerror("ioctl I_PUSH ldterm");
    if(ioctl(*ttyfd, I_PUSH, "ttcompat")<0)
        ioerror("ioctl I_PUSH ttcompat");
    return 0;
#else /* HAVE_DEV_PTMX */
#ifdef HAVE_DEV_PTS_AND_PTC
    /* AIX-style pty code. */
    const char *name;

    *ptyfd=open("/dev/ptc", O_RDWR|O_NOCTTY);
    if(*ptyfd<0) {
        ioerror("open(/dev/ptc)");
        return -1;
    }
    name=ttyname(*ptyfd);
    if(!name) {
        s_log(LOG_ERR, "Open of /dev/ptc returns device for which ttyname fails");
        return -1;
    }
    strcpy(namebuf, name);
    *ttyfd=open(name, O_RDWR|O_NOCTTY);
    if(*ttyfd<0) {
        ioerror(name);
        close(*ptyfd);
        return -1;
    }
    return 0;
#else /* HAVE_DEV_PTS_AND_PTC */
    /* BSD-style pty code. */
    char buf[64];
    int i;
    const char *ptymajors="pqrstuvwxyzabcdefghijklmnoABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char *ptyminors="0123456789abcdef";
    int num_minors=strlen(ptyminors);
    int num_ptys=strlen(ptymajors)*num_minors;

    for(i=0; i<num_ptys; i++) {
#ifdef HAVE_SNPRINTF
        snprintf(buf, sizeof buf,
#else
        sprintf(buf,
#endif
             "/dev/pty%c%c", ptymajors[i/num_minors],
             ptyminors[i%num_minors]);
        *ptyfd=open(buf, O_RDWR|O_NOCTTY);
        if(*ptyfd<0)
            continue;
#ifdef HAVE_SNPRINTF
        snprintf(namebuf, 64,
#else
        sprintf(namebuf,
#endif
            "/dev/tty%c%c",
            ptymajors[i/num_minors], ptyminors[i%num_minors]);

        /* open the slave side */
        *ttyfd=open(namebuf, O_RDWR | O_NOCTTY);
        if(*ttyfd<0) {
            ioerror(namebuf);
            close(*ptyfd);
            return -1;
        }
        return 0;
    }
    return -1;
#endif /* HAVE_DEV_PTS_AND_PTC */
#endif /* HAVE_DEV_PTMX */
#endif /* HAVE__GETPTY */
#endif /* HAVE_OPENPTY */
}
