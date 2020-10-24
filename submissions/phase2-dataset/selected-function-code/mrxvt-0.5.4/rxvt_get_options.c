void
rxvt_get_options(rxvt_t *r, int argc, const char *const *argv)
{
    int		 i, bad_option = 0;
    static const char On[3] = "ON", Off[4] = "OFF";


    rxvt_dbgmsg ((DBG_DEBUG, DBG_RESOURCE, "rxvt_get_options()\n"));

    for (i = 1; i < argc; i++)
    {
	unsigned int	entry, longopt = 0;
	const char	*flag, *opt;
	int		profileNum;

	opt = argv[i];
	profileNum = 0;	/* initialize profileNum to 0 by default */

	rxvt_msg (DBG_INFO, DBG_RESOURCE, "argv[%d] = %s: \n", i, opt);
	if (*opt == '-')
	{
	    flag = On;
	    if (*++opt == '-')
		longopt = *opt++;   /* long option */
	}
	else if (*opt == '+')
	{
	    flag = Off;
	    if (*++opt == '+')
		longopt = *opt++;   /* long option */
	}
	else
	{
	    bad_option = 1;
	    rxvt_msg (DBG_ERROR, DBG_RESOURCE, "bad option \"%s\"\n", opt);
	    continue;
	}

	if (!STRCMP(opt, "help"))
	    rxvt_usage(longopt ? 2 : 1);
	if (!STRCMP(opt, "h"))
	    rxvt_usage(0);

	/* feature: always try to match long-options */
	for (entry = 0; entry < optList_size(); entry++)
	{
	    char	    buflong[256];
	    char	    bufshort[128];

	    /* initialize it to empty string */
	    buflong[0]	= '\0';
	    bufshort[0] = '\0';

	    /*
	     * Get the long option name in buflong, or the short option name in
	     * bufshort.
	     */
	    if ( optList[entry].multiple )
	    {
		int offset = 0;

		/*
		 * For backward compatibility, accept vt%d style options.
		 */
		offset = rxvt_str_match( opt, "vt" );
		if( offset == 0 )
		    offset = rxvt_str_match( opt, "profile" );

		/*
		 * Copy --profile%d.resource or --vt%d.resource into buflong and
		 * bufshort.
		 */
		if( offset )
		{
		    profileNum = atoi( opt + offset );
		    if( profileNum < 0 || profileNum >= MAX_PROFILES )
		    {
			entry = optList_size();
			break;	/* out of range, jump to bad option */
		    }

		    snprintf( buflong, sizeof(buflong)-1, "%.*s%d.%s",
			    offset, opt, profileNum, optList[entry].kw );
		    buflong[sizeof(buflong)-1] = '\0';

		    snprintf( bufshort, sizeof(bufshort)-1, "%.*s%d.%s",
			    offset, opt, profileNum, optList[entry].opt );
		    bufshort[sizeof(bufshort)-1] = '\0';

		    rxvt_dbgmsg ((DBG_DEBUG, DBG_RESOURCE, "Matched profile=%d buflong=%s bufshort=%s\n", profileNum, buflong, bufshort ));
		}

		/* If no profile number is specified, use 0 by default */
		else
		{
		    STRNCPY( buflong, optList[entry].kw, sizeof(buflong) - 1 );
		    buflong[ sizeof(buflong)-1 ] = '\0';

		    STRNCPY( bufshort, optList[entry].opt, sizeof(bufshort)-1 );
		    bufshort[ sizeof(bufshort)-1 ] = '\0';

		    profileNum = 0;

		    rxvt_dbgmsg ((DBG_DEBUG, DBG_RESOURCE, "Matched default buflong=%s bufshort=%s\n", buflong, bufshort ));
		}
	    }
	    else if (optList[entry].kw)
	    {
		STRNCPY (buflong, optList[entry].kw, sizeof(buflong)-1);
		buflong[sizeof(buflong)-1] = '\0';
		if (optList[entry].opt)
		{
		    STRNCPY (bufshort, optList[entry].opt,
			sizeof(bufshort)-1);
		    bufshort[sizeof(bufshort)-1] = '\0';
		}
	    }
	    else if (optList[entry].opt)
	    {
		/* here NULL == optList[entry].kw */
		STRNCPY (bufshort, optList[entry].opt,
		    sizeof(bufshort)-1);
		bufshort[sizeof(bufshort)-1] = '\0';
	    }


	    /* Check to see if option matches */
	    if(
		( optList[entry].kw && !STRCMP(opt, buflong) )
		|| (
		     !longopt && optList[entry].opt
		     && !STRCASECMP(opt, bufshort)
		   )
	      )
		break;
	} /* for */

	/* Found option */
	if (entry < optList_size())
	{
	    if (optList_isReverse(entry))
		flag = (flag == On) ? Off : On;

	    /* string value */
	    if( optList_STRLEN(entry) )
	    {
		const char   *str = argv[++i];

		rxvt_dbgmsg ((DBG_DEBUG, DBG_RESOURCE, "string (%s,%s) = ", optList[entry].opt ? optList[entry].opt : "nil", optList[entry].kw ? optList[entry].kw : "nil"));

		if (flag == On && str && (optList[entry].doff != -1))
		{
		    rxvt_dbgmsg ((DBG_DEBUG, DBG_RESOURCE, "\"%s\"\n", str));
		    r->h->rs[optList[entry].doff + profileNum] = str;
		    /*
		     * special cases are handled in main.c:main() to allow X
		     * resources to set these values before we settle for
		     * default values
		     */
		}
		else
		    rxvt_dbgmsg ((DBG_DEBUG, DBG_RESOURCE, "???\n"));
	    }

	    /* boolean value */
	    else
	    {
		rxvt_dbgmsg ((DBG_DEBUG, DBG_RESOURCE, "boolean (%s,%s) = %s\n", optList[entry].opt, optList[entry].kw, flag ));

		if( flag == On )
		    SET_OPTION( r, optList[entry].flag );
		else
		    UNSET_OPTION( r, optList[entry].flag );

		/*
		 * Remember that we've set this option, so it does not get reset
		 * later.
		 */
		SET_ARRAYOPT( pSetOpts, optList[entry].flag );
	    }
	}

	/* No option found */
	else
	{
	    if ( rxvt_str_match( opt, "macro."))
	    {
		const char  *str = argv[++i];

		if (NOT_NULL(str))
		    rxvt_parse_macros( r, opt + sizeof( "macro." ) - 1,
			    str, 0); /* priority 0 = highest. All previous
					macros get replaced. */
	    }
	    else
	    {
		/*
		 * various old-style options, just ignore
		 * Obsolete since about Jan 96,
		 * so they can probably eventually be removed
		 */
		const char   *msg = "bad";

		if (longopt)
		{
		    opt--;
		    bad_option = 1;
		}
		else if (!STRCMP(opt, "7") || !STRCMP(opt, "8")
#ifdef GREEK_SUPPORT
		   /* obsolete 12 May 1996 (v2.17) */
		   || rxvt_str_match(opt, "grk")
#endif
		)
		    msg = "obsolete";
		else
		    bad_option = 1;

		rxvt_msg (DBG_ERROR, DBG_RESOURCE, "%s option \"%s\"", msg, --opt);
	    }
	}
    }

    if (bad_option)
    {
	/* Printing the usage is too verbose ... */
#if 0
	rxvt_usage(0);
#endif

	rxvt_msg (DBG_ERROR, DBG_RESOURCE,
		"Use -h, -help or --help to get help\n" );
	exit( EXIT_FAILURE );
    }

    rxvt_dbgmsg ((DBG_DEBUG, DBG_RESOURCE, "rxvt_get_options() done.\n"));
}