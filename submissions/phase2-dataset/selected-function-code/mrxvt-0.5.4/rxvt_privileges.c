void
rxvt_privileges(int mode)
{
#if !defined(OS_CYGWIN)
# if !defined(HAVE_SETEUID) && defined(HAVE_SETREUID)
/* setreuid() is the poor man's setuid(), seteuid() */
#  define seteuid(a)	setreuid(-1, (a))
#  define setegid(a)	setregid(-1, (a))
#  define HAVE_SETEUID
# endif
# ifdef HAVE_SETEUID
    switch (mode)
    {
	case IGNORE:
	/*
	 * change effective uid/gid - not real uid/gid - so we can switch
	 * back to root later, as required
	 */
	    seteuid(getuid());
	    setegid(getgid());
	    break;
	case SAVE:
	    g_euid = geteuid();
	    g_egid = getegid();
	    break;
	case RESTORE:
	    seteuid(g_euid);
	    setegid(g_egid);
	    break;
    }
# else
    switch (mode)
    {
	case IGNORE:
	    if (setuid(getuid()) < 0)
		exit (EXIT_FAILURE);
	    if (setgid(getgid()) < 0)
		exit (EXIT_FAILURE);
	    /* FALLTHROUGH */
	case SAVE:
	    /* FALLTHROUGH */
	case RESTORE:
	    break;
    }
# endif
#endif
}