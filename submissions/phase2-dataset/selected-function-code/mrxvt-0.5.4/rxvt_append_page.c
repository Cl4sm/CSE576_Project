void
rxvt_append_page( rxvt_t* r, int profile,
	const char TAINTED *title, const char *command )
{
    int	    num_cmd_args = 0; /* Number of args we got from parsing command */
    char**  argv;


    rxvt_dbgmsg ((DBG_DEBUG, DBG_TABBAR, "rxvt_append_page( r, %d, %s, %s )\n", profile, title ? title : "(nil)", command ? command : "(nil)" ));

    /* Sanitization */
    assert( LTAB(r) < MAX_PAGES );

    if (LTAB(r) == MAX_PAGES-1)
    {
	rxvt_msg (DBG_ERROR, DBG_TABBAR,  "Too many tabs" );
	return ;
    }

    if( profile < 0 || profile >= MAX_PROFILES )
    {
	rxvt_msg (DBG_WARN, DBG_TABBAR,  "Warning: Profile %d out of range", profile );
	profile = 0;
    }

    /* indicate that we add a new tab */
    LTAB(r)++;
    rxvt_dbgmsg ((DBG_VERBOSE, DBG_TABBAR, "last page is %d\n", LTAB(r)));

    /*
     * Use command specified with -e only if we're opening the first tab, or the
     * --cmdAllTabs option is specified, and we're not given a command to
     *  execute (e.g. via the NewTab cmd macro).
     */
    if(
	 cmd_argv	    /* Argument specified via -e option */
	 && command == NULL /* No command specified (e.g. via NewTab macro) */
	 && (
	       LTAB(r)== 0			    /* First tab */
	       || ISSET_OPTION(r, Opt2_cmdAllTabs)  /* -at option */
	    )
      )
	argv = cmd_argv;

    else
    {
	/* load tab command if necessary*/
	if( command == NULL )
	    command = getProfileOption( r, profile, Rs_command );

	if( command != NULL && *command != '!' )
	{
	    const char *cmd = command;

	    /* If "command" starts with '!', we should run it in the shell. */
	    if( cmd[0] == '\\' && cmd[1] == '!' )
		cmd++;

	    argv = rxvt_string_to_argv( cmd, &num_cmd_args );
	}
	else
	    argv = NULL;
    }
    rxvt_dbgmsg ((DBG_DEBUG, DBG_TABBAR, "Forking command=%s, argv[0]=%s\n", command ? command : "(nil)", ( argv && argv[0] ) ? argv[0] : "(nil)" ));

    /*
     * Set the tab title.
     */
    if( title == NULL || *title == '\0' )
    {
	title = getProfileOption( r, profile, Rs_tabtitle );
	if( title == NULL || *title == '\0' )
	{
	    if( command && *command != '\0' )
		title = command;
	    else if( argv && argv[0] && *argv[0] != '\0' )
		title = argv[0];
	}
    }
    rxvt_create_termwin( r, LTAB(r), profile, title );

    /*
     * Run the child process.
     *
     * 2006-02-17 gi1242: Bug -- If the child produces some output and exits
     * quickly, then some of that output is sometimes lost.
     */
    if( getProfileOption( r, profile, Rs_cwd ) != NULL )
    {
	const char  *cwdOption	= getProfileOption( r, profile, Rs_cwd );
	char	    cwd[PATH_MAX] = "",
		    child_cwd[PATH_MAX] = "";
	int	    len = 0;


	getcwd (cwd, PATH_MAX);

	if( !STRCMP( cwdOption, "." ) )
	{
	    if( ATAB(r) != LTAB(r) )
	    {
		/*
		 * Copy working directory of the current tab into child_cwd.
		 */
		char	proc_cwd[32];		/* 16 is enough */

		sprintf( proc_cwd, "/proc/%d/cwd", AVTS(r)->cmd_pid );
		if( (len = readlink( proc_cwd, child_cwd, PATH_MAX-1) ) > 0 )
		    /* readlink does not null terminate */
		    child_cwd[len] = 0;
	    }
	}
	
	else
	{
#ifdef HAVE_WORDEXP_H
	    wordexp_t p;
	    int wordexp_result = wordexp(cwdOption, &p, 0);
	    char *filename;

	    if( wordexp_result == 0 )
	    {
		if( p.we_wordc > 1)
		    rxvt_msg( DBG_ERROR, DBG_TABBAR,
			    "Too many words when expanding %s\n", cwdOption );
		else
		{
		    filename = *p.we_wordv;

		    len  = STRLEN( filename );
		    MIN_IT( len, PATH_MAX - 1 );
		    STRNCPY( child_cwd, filename, len );
		    child_cwd[len] = 0;
		}

		wordfree( &p );
	    }

	    else
	    {
		rxvt_dbgmsg(( DBG_VERBOSE, DBG_TABBAR,
			    "wordexp error code '%i', expanding '%s'\n",
			    wordexp_result, filename ));
	    }
#endif /* HAVE_WORDEXP_H */
	}

	if( len > 0 && chdir( child_cwd ) == 0 )
	{
	    /* Now in working directory of ATAB */
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_TABBAR,
			"Running child in directory: %s\n", child_cwd ));

	    /* Run command in this new directory. */
	    LVTS(r)->cmd_fd =
		rxvt_run_command( r, LTAB(r), (const char**) argv );

	    /* Restore old working directory. */
	    chdir( cwd );
	}

	else
	{
	    /* Exec command in original directory. */
	    rxvt_dbgmsg(( DBG_DEBUG, DBG_TABBAR,
			"Running child in original directory\n"));

	    LVTS(r)->cmd_fd =
		rxvt_run_command( r, LTAB(r), (const char**) argv );
	}
    }
    else
	LVTS(r)->cmd_fd = rxvt_run_command (r, LTAB(r), (const char**) argv);


    /*
     * In case we allocated memory for argv using rxvt_string_to_argv (because a
     * command was specified), then free it.
     */
    if( num_cmd_args > 0)
    {
	char **s;

	for( s = argv; *s != NULL; s++) rxvt_free(*s);
	rxvt_free( argv );
    }

    /*
     * If run command failed, rollback
     */
    assert( -1 != LVTS(r)->cmd_fd );
    if (-1 == LVTS(r)->cmd_fd)
    {
	rxvt_destroy_termwin (r, LTAB(r));
	LTAB(r) --;
	return;
    }
    rxvt_dbgmsg ((DBG_DEBUG, DBG_TABBAR,"page %d's cmd_fd is %d\n", LTAB(r), LVTS(r)->cmd_fd));


    /* adjust number of file descriptors to listen */
    rxvt_adjust_fd_number (r);

    /*
     * Initialize the screen data structures
     */
    rxvt_scr_reset (r, LTAB(r));
    rxvt_scr_refresh (r, LTAB(r), FAST_REFRESH);

    /*
     * Now we actually execute the command after executing shell, but we need
     * careful check first.
     */
    if( command != NULL && *command == '!' )
    {
	command++;  /* Skip leading '!' */
	rxvt_tt_write( r, LTAB(r), (const unsigned char*) command,
		STRLEN(command) );
	rxvt_tt_write( r, LTAB(r), (const unsigned char*) "\n", 1 );
    }

    /*
     * Now update active page information
     */
    PTAB(r) = ATAB(r); /* set last previous tab */
    ATAB(r) = LTAB(r); /* set the active tab */

    /* update mapped flag */
    AVTS(r)->mapped = 1;

    /* first tab is special since ptab = atab now */
    if (PTAB(r) != ATAB(r))
	PVTS(r, r->tabBar.ptab)->mapped = 0;

    /* Adjust visible tabs */
    rxvt_tabbar_set_visible_tabs (r, True); /* Send expose events to tabbar */
    refresh_tabbar_tab( r, PTAB(r));	    /* PTAB will need to be drawn as
					       inactive */

    /*
     * Auto show tabbar if we have exactly two tabs.
     */
    if(
	 !r->tabBar.state && LTAB(r) == 1
	 && ISSET_OPTION(r, Opt2_autohideTabbar)
	 && rxvt_tabbar_show( r )
      )
	rxvt_resize_on_subwin( r, SHOW_TABBAR);

    /* synchronize terminal title with tab title */
    if (ISSET_OPTION(r, Opt2_syncTabTitle))
	sync_tab_title( r, ATAB(r) );

    /* synchronize icon name to tab title */
    if (ISSET_OPTION(r, Opt2_syncTabIcon))
	rxvt_set_icon_name (r,
		(const unsigned char*) PVTS(r, ATAB(r))->tab_title);
}