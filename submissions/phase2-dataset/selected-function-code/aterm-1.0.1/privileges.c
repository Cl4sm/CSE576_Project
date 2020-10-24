privileges(int mode)
{
#ifdef HAVE_SETEUID
    static uid_t    euid;
    static gid_t    egid;

    switch (mode) {
    case IGNORE:
    /*
     * change effective uid/gid - not real uid/gid - so we can switch
     * back to root later, as required
     */
	seteuid(getuid());
	setegid(getgid());
	break;

    case SAVE:
	euid = geteuid();
	egid = getegid();
	break;

    case RESTORE:
	seteuid(euid);
	setegid(egid);
	break;
    }
#else
# ifndef __CYGWIN32__
    switch (mode) {
    case IGNORE:
	setuid(getuid());
	setgid(getgid());
	break;

    case SAVE:
	break;
    case RESTORE:
	break;
    }
# endif
#endif
}
