void
rxvt_init_env(rxvt_t *r)
{
    int		    i;
    unsigned int    u;
    char*	    val;


#ifdef DISPLAY_IS_IP
    /* Fixup display_name for export over pty to any interested
    ** terminal clients via "ESC[7n" (e.g. shells).  Note we use
    ** the pure IP number (for the first non-loopback interface)
    ** that we get from rxvt_network_display(). This is more
    ** "name-resolution-portable", if you will, and probably allows
    ** for faster x-client startup if your name server is beyond
    ** a slow link or overloaded at client startup. Of course that
    ** only helps the shell's child processes, not us.
    **
    ** Giving out the display_name also affords a potential
    ** security hole
    */
    val = rxvt_network_display(r->h->rs[Rs_display_name]);
    r->h->rs[Rs_display_name] = (const char *)val;
    if (IS_NULL(val))
#endif		    /* DISPLAY_IS_IP */
    val = XDisplayString(r->Xdisplay);
    if (IS_NULL(r->h->rs[Rs_display_name]))
	r->h->rs[Rs_display_name] = val;    /* use broken `:0' value */

    i = STRLEN(val) + 9;
    if (i <= 0 || i > 1024) /* possible integer overflow */
	i = 1024;
    r->h->env_display = rxvt_malloc(i * sizeof(char));
    STRCPY (r->h->env_display, "DISPLAY=");
    STRNCAT (r->h->env_display, val, i-9);
    r->h->env_display[i-1] = (char) 0;

    /* avoiding the math library:
     * i = (int)(ceil(log10((unsigned int)r->TermWin.parent))) */
    for (i = 0, u = (unsigned int)r->TermWin.parent; u; u /= 10, i++)
	;
    MAX_IT(i, 1);
    r->h->env_windowid = rxvt_malloc((i + 10) * sizeof(char));

    sprintf(r->h->env_windowid, "WINDOWID=%u",
	(unsigned int)r->TermWin.parent);

    /*
    ** add entries to the environment:
    ** @ DISPLAY:   in case we started with -display
    ** @ WINDOWID:  X window id number of the window
    ** @ COLORTERM: terminal sub-name and also indicates its color
    ** @ TERM:	    terminal name
    ** @ TERMINFO:  path to terminfo directory
    */
#ifdef HAVE_PUTENV
    putenv(r->h->env_display);
    putenv(r->h->env_windowid);

# ifdef RXVT_TERMINFO
    putenv("TERMINFO=" RXVT_TERMINFO);
# endif
    if (XDEPTH <= 2)
	putenv("COLORTERM=" COLORTERMENV "-mono");
    else
	putenv("COLORTERM=" COLORTERMENVFULL);
    if (NOT_NULL(r->h->rs[Rs_term_name]))
    {
	int	l = 6 + STRLEN(r->h->rs[Rs_term_name]);
	if (l <= 0 || l > 1024)	/* possible integer overflow */
	    l = 1024;
	r->h->env_term = rxvt_malloc(l * sizeof(char));
	STRCPY (r->h->env_term, "TERM=");
	STRNCAT (r->h->env_term, r->h->rs[Rs_term_name], l-6);
	r->h->env_term[l-1] = (char) 0;
	putenv(r->h->env_term);
    }
    else
	putenv("TERM=" TERMENV);
#endif	/* HAVE_PUTENV */

#ifdef HAVE_UNSETENV
    /* avoid passing old settings and confusing term size */
    unsetenv("LINES");
    unsetenv("COLUMNS");
    unsetenv("TERMCAP");    /* terminfo should be okay */
#endif		    /* HAVE_UNSETENV */

    /*
    ** allocate environment variable for MRXVT_TABTITLE, we will
    ** use it in rxvt_create_termwin later for each tab terminal
    */
    r->h->env_tabtitle = rxvt_malloc(sizeof(TABTITLEENV) + MAX_TAB_TXT + 1);
}