void
rxvt_get_xdefaults(rxvt_t *r, FILE *stream, const char *name,
	macro_priority_t priority)
{
    unsigned int    len, lenstr;
    char TAINTED *  str;
    char	    buffer[256];

    rxvt_msg (DBG_INFO, DBG_RESOURCE, "rxvt_get_xdefaults (%s)\n", name);

    if (IS_NULL(stream))
	return;
    len = STRLEN(name);

    while (NOT_NULL(str = fgets(buffer, sizeof(buffer), stream)))
    {
	unsigned int	entry, n;

	while (*str && isspace((int) *str))
	    str++;	/* leading whitespace */

	if(
	    (str[len] != '*' && str[len] != '.')
	    || (len && STRNCMP(str, name, len))
	  )
	    continue;
	str += (len + 1);   /* skip `name*' or `name.' */
	lenstr = STRLEN( str );

	{
	    /*
	     * replace '*' with '.', but stop at ':'! bug reported by
	     * afo@zlug.org
	     */
	    char*   ptr = str;
	    while (*ptr && *ptr != ':')
	    {
		if ('*' == *ptr)
		    *ptr = '.';
		ptr ++;
	    }
	}

	if( !rxvt_parse_macros( r, str, NULL, priority ) )
	{
	    for (entry = 0; entry < optList_size(); entry++)
	    {
		/* const char*	kw = optList[entry].kw; */
		char	kw[256];
		int	profileNum = 0;	/* default is no offset */

		if (IS_NULL(optList[entry].kw))
		    continue;
		STRNCPY (kw, optList[entry].kw, sizeof(kw)-1);
		kw[sizeof(kw)-1] = (char) 0;

		if( optList[entry].multiple )
		{
		    int offset = 0;

		    /*
		     * For backward compatibility, accept vt%d style options.
		     */
		    offset = rxvt_str_match( str, "vt" );
		    if( offset == 0 )
			offset = rxvt_str_match( str, "profile" );

		    /*
		     * Copy profile%d.resource into kw.
		     */
		    if( offset )
		    {
			char	buf[256];

			profileNum = atoi( str + offset );
			if (profileNum < 0 || profileNum >= MAX_PROFILES)
			    continue;	/* out of range */

			snprintf( buf, sizeof(buf)-1,
				"%.*s%d.%s", offset, str, profileNum, kw );
			buf[sizeof(buf)-1] = '\0';

			STRNCPY (kw, buf, sizeof(kw)-1);
			kw[sizeof(kw)-1] = '\0';

			rxvt_dbgmsg ((DBG_DEBUG, DBG_RESOURCE, "Matched profile=%d kw=%s\n", profileNum, kw));
		    }

		    /*
		     * No profile%d specified. Match resource on it's own, and
		     * use profile 0.
		     */
		    else
		    {
			profileNum = 0;

			rxvt_dbgmsg ((DBG_DEBUG, DBG_RESOURCE, "Matched default kw=%s for option %s", kw, str));
		    }
		}

		n = STRLEN(kw);
		if( n < lenstr && str[n] == ':' && rxvt_str_match(str, kw) )
		{
		    /* skip `keyword:' */
		    str += (n + 1);
		    rxvt_str_trim(str);
		    n = STRLEN(str);

		    /*
		     * Only set the resource if it's not already set. We read
		     * config file items in reverse order (most significant to
		     * least significant). So if a resource is already set, then
		     * we should not over ride it.
		     */
		    if( optList_isBool( entry ) )
		    {
			/*
			 * Boolean options: We dont' need to duplicate the
			 * argument of the resource string. That only wastes
			 * memory.
			 */
			if( NOTSET_ARRAYOPT( pSetOpts, optList[entry].flag ) )
			{
			    /*
			     * Only set this option if we've not already set it.
			     */
			    int	s;

			    s = STRCASECMP(str, "true") == 0 ||
				STRCASECMP(str, "yes") == 0 ||
				STRCASECMP(str, "on") == 0 ||
				STRCASECMP(str, "1") == 0;
			    if (optList_isReverse(entry))
				s = !s;

			    if (s)
				SET_OPTION(r, optList[entry].flag);
			    else
				UNSET_OPTION(r, optList[entry].flag);

			    /* Remember we've already set this option */
			    SET_ARRAYOPT( pSetOpts, optList[entry].flag );
			}
		    }

		    else if( IS_NULL( r->h->rs[ optList[entry].doff
							    + profileNum ] ) )
		    {
			/*
			 * Regular option, that has not previously been set.
			 * The argument needs to be duplicated and stored.
			 */
			r->h->rs[optList[entry].doff+profileNum] =
			    n && !optList_isBool(entry) ?
				STRDUP( str ) : emptyResource;
		    }

		    break; /* out of for( entry=0 ... ) */

		}   /* if( str[n] =':' ...) */
	    }	/* for (entry = 0...) */
	}   /* if( !rxvt_parse_macros ... ) */
    }	/* while() */

    rewind( stream );
}