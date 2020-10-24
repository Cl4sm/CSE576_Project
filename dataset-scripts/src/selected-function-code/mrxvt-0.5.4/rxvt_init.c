rxvt_t		*
rxvt_init(int argc, const char *const *argv)
{
    register int    i;
    register int    itnum; /* initial terminal number */
    rxvt_t*	    r;

    /* Allocate memory for "r" and initialize contents to 0 */
    r = (rxvt_t *) rxvt_calloc(1, sizeof(rxvt_t));

    /* Save "r" in _rxvt_vars. This is the only assignment to _rxvt_vars */
    rxvt_set_r(r);

    /* Initialize vars in "r" */
    if (rxvt_init_vars(r) < 0)
    {
	rxvt_msg (DBG_ERROR, DBG_MAIN,  "Could not initialize." );
	rxvt_free(r);
	return NULL;
    }

    /* save global argc and argv */
    r->global_argc = argc;
    r->global_argv = (char**) argv;

    rxvt_init_secondary(r);
    cmd_argv = rxvt_init_resources(r, argc, argv);

    rxvt_pre_show_init( r );

    rxvt_create_show_windows(r, argc, argv);

#ifdef TRANSPARENT
    if (ISSET_OPTION(r, Opt_transparent))
    {
	XSelectInput(r->Xdisplay, XROOT, PropertyChangeMask);
	/*
	 * Our "parents" will automatically be checked on the first expose and
	 * ConfigureNotify event respectively. Forcefully calling it is just a
	 * waste of time.
	 */
# if 0
	rxvt_check_our_parents(r);
# endif
    }
#endif

    rxvt_init_env(r);
    rxvt_init_command(r);
    rxvt_init_screen (r);

    /*
     * Initialize the pages
     */
    if( r->h->rs[Rs_initProfiles] )
    {
	/* Split into a comma separated string */
	char *s = (char *) r->h->rs[Rs_initProfiles];

	do
	  {
	    int profile = atoi( s );

	    rxvt_append_page( r, profile, NULL, NULL );

	    s = STRCHR( s, ',' );
	  }
	while (NULL != s++);
    }


    /* Backward compatibility: Open profiles 0 .. n-1 if tnum=n. */
    else if( r->h->rs[Rs_init_term_num] )
    {
	rxvt_msg (DBG_ERROR, DBG_MAIN,  "Option tnum is obsolete."
		" Use --initProfileList instead" );

	itnum = atoi( r->h->rs[Rs_init_term_num] );
	itnum = max( 1, itnum );
	itnum = min( itnum, MAX_PAGES );

	for (i = 0; i < itnum; i ++)
	    rxvt_append_page( r, (i < MAX_PROFILES) ? i : 0 , NULL, NULL );
    }

    /* Just open the default tab */
    else
	rxvt_append_page( r, 0, NULL, NULL );

    /* Activate the tab */
    rxvt_activate_page (r, 0);

    /* Initialize xlocale after VT is created */
    rxvt_init_xlocale(r);

    return r;
}