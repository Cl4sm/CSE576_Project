void
rxvt_init_secondary(rxvt_t *r)
{
    int		i, num_fds;
#ifdef TTY_GID_SUPPORT
    struct group   *gr = getgrnam("tty");

    if (gr)	    /* change group ownership of tty to "tty" */
    {
	r->h->ttygid = gr->gr_gid;
    }
    else
#endif		    /* TTY_GID_SUPPORT */
    {
	r->h->ttygid = getgid();
    }

    rxvt_set_default_locale (r);

    /* get number of available file descriptors */
#if defined(_POSIX_VERSION) || ! defined(OS_SVR4)
    num_fds = (int)sysconf(_SC_OPEN_MAX);
#else
    num_fds = rxvt_getdtablesize();
#endif

    /*
    ** Close all unused file descriptors
    ** We don't want them, we don't need them.
    */
    if ((i = open("/dev/null", O_RDONLY)) < 0)
    {
	/* TODO: BOO HISS */
	dup2(STDERR_FILENO, STDIN_FILENO);
    }
    else if (i > STDIN_FILENO)
    {
	dup2(i, STDIN_FILENO);
	close(i);
    }
    dup2(STDERR_FILENO, STDOUT_FILENO);
    for (i = STDERR_FILENO + 1; i < num_fds; i++)
    {
    /* #ifdef __sgi */
#ifdef OS_IRIX
	/* Alex Coventry says we need 4 & 7 too */
	if (i == 4 || i == 7)
	    continue;
#endif
	close(i);
    }

    /* Now set the correct num_fds */
    r->num_fds = STDERR_FILENO + 1;
#ifdef OS_IRIX
    r->num_fds = 7 + 1;
#endif
}