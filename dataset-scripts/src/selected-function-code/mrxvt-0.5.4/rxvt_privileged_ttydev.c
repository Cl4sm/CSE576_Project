void
rxvt_privileged_ttydev(rxvt_t* r, int page, char action)
{
    rxvt_msg (DBG_INFO, DBG_MAIN, "rxvt_privileged_ttydev %d (r, %c); waiting for: %c (pid: %d)\n", page, action, PVTS(r, page)->next_tty_action, getpid());
    if (PVTS(r, page)->next_tty_action != action ||
	(action != SAVE && action != RESTORE) ||
	IS_NULL(PVTS(r, page)->ttydev) ||
	(char) 0 == *(PVTS(r, page)->ttydev))
	return;

    rxvt_privileges(RESTORE);

    if (action == SAVE)
    {
	PVTS(r, page)->next_tty_action = RESTORE;
# ifndef RESET_TTY_TO_COMMON_DEFAULTS
	/*
	 * store original tty status for restoration rxvt_clean_exit() -- rgg
	 * 04/12/95
	 */
	if (lstat(PVTS(r, page)->ttydev, &h->ttyfd_stat) < 0) /* you lose out */
	    PVTS(r, page)->next_tty_action = IGNORE;
	else
# endif
	{
	    /* fail silently */
	    chown(PVTS(r, page)->ttydev, getuid(), r->h->ttygid);
	    chmod(PVTS(r, page)->ttydev, PVTS(r, page)->ttymode);
# ifdef HAVE_REVOKE
	    revoke(PVTS(r, page)->ttydev);
# endif
	}
    }
    else		    /* action == RESTORE */
    {
	PVTS(r, page)->next_tty_action = IGNORE;
# ifndef RESET_TTY_TO_COMMON_DEFAULTS
	chmod(PVTS(r, page)->ttydev, PVTS(r, page)->ttyfd_stat.st_mode);
	chown(PVTS(r, page)->ttydev, PVTS(r, page)->ttyfd_stat.st_uid, PVTS(r, page)->ttyfd_stat.st_gid);
# else
	chmod(PVTS(r, page)->ttydev,
	    (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH));
	chown(PVTS(r, page)->ttydev, 0, 0);
# endif
    }

    rxvt_privileges(IGNORE);

# ifndef RESET_TTY_TO_COMMON_DEFAULTS
    rxvt_msg (DBG_INFO, DBG_MAIN, "%s \"%s\": mode %03o, uid %d, gid %d\n", action == RESTORE ? "Restoring" : (action == SAVE ? "Saving" : "UNKNOWN ERROR for"), PVTS(r, page)->ttydev, PVTS(r, page)->ttyfd_stat.st_mode, PVTS(r, page)->ttyfd_stat.st_uid, PVTS(r, page)->ttyfd_stat.st_gid);
# endif
}