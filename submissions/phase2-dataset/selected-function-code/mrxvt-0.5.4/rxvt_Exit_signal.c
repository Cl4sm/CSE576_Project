RETSIGTYPE
rxvt_Exit_signal(int sig)
{
#ifdef UTMP_SUPPORT
    register int    i;
#endif
    rxvt_t*	    r;

    rxvt_msg (DBG_INFO, DBG_MAIN, "Received signal %d\n", (int) sig);

    r = rxvt_get_r();

#ifdef UTMP_SUPPORT
    for (i = 0; i <= LTAB(r); i ++)
    {
	rxvt_privileges( RESTORE );
	rxvt_cleanutent( r, i );
	rxvt_privileges( IGNORE );
    }
#endif

    /* resend signal to default handler */
    /* kill (getpid (), sig); */
    rxvt_close_all_tabs( r );
}