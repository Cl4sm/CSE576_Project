char**
rxvt_string_to_argv( const char *string, int *argc )
{
    int		    i = 0;
    char**	    pret;
    const char*	    pcur;
#ifdef INTERNAL_ARGV_SPLIT
    const char*	    pbeg;
#endif

    *argc = 0;
    if( IS_NULL(string) || *string == '\0' )
    {
	*argc = 0;
	return NULL;
    }

#define MAX_ARGV    (1024)
    /* Up to 64 argv.
     *
     * 2006-02-23 gi1242: Use calloc instead of malloc. Thus when freeing pret,
     * we can safely free all elements till we encounter a NULL pointer.
     */
    pret = (char**) rxvt_calloc (MAX_ARGV, sizeof (char*));

    rxvt_dbgmsg ((DBG_DEBUG, DBG_INIT, "fetch command argv for the tab\n"));
#ifdef INTERNAL_ARGV_SPLIT
    /*{{{*/
    /*
     * 2006-02-23 gi1242: Remember to leave space for a NULL terminated pointer
     * at the end
     */
    pbeg = pcur = string;
    for (i = 0; i < MAX_ARGV-2; i ++)
    {
	int	dq = 0;	/* double quote */
	int	sq = 0;	/* single quote */
	/* set default argument to NULL */
	SET_NULL(pret[i]);

	/* skip any spaces and non-printable */
	while (*pcur && 
	    (isspace ((int) *pcur) || !isprint ((int) *pcur)))
	    pcur ++;
	/* stop if reach end of string */
	if (!*pcur)
	    break;

	/* beginning of the token */
	if (isalnum ((int) *pcur) || ispunct ((int) *pcur))
	{
	    if ('\"' == *pcur)
	    {
		/* beginning of double quote */
		dq = 1;	pbeg = pcur + 1; pcur ++;
	    }
	    else if ('\'' == *pcur)
	    {
		/* beginning of single quote */
		sq = 1;	pbeg = pcur + 1; pcur ++;
	    }
	    else /* normal characters */
		pbeg = pcur;
	}
#ifdef DEBUG
	else	/* shouldn't happen */
	    assert (0);
#endif

	/* move forward one character */
	pcur ++;

	/* now fetch the new token */
	while(
		*pcur &&			/* not end of string */
		(
		  (dq && *pcur != '\"') ||	/* not end of double quote */
		  (sq && *pcur != '\'') ||	/* not end of single quote */
		  (!dq && !sq && !isspace ((int) *pcur))
		)
	     )
	{
	    pcur ++;
	}

	if (!*pcur &&	/* end of string */
	    (dq || sq))	/* no match of quote is found */
	    goto NotMatch;

	if (!*pcur)	/* end of string */
	{
	    pret[i] = STRDUP (pbeg);
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_INIT, "   argv[%d] = %s\n", i, pret[i]));
	    break;	/* ready to return */
	}

	if (
	      (dq && *pcur == '\"')		    /* end of double quote */
	      || (sq && *pcur == '\'')		    /* end of single quote */
	      || (!dq && !sq && isspace ((int) *pcur))	    /* space */
	   )
	{
	    int	    len = sizeof (char) * (pcur - pbeg) + 1;

	    assert (len > 0);	/* possible integer overflow? */
	    pret[i] = (char*) rxvt_malloc (len * sizeof(char));
	    MEMCPY (pret[i], pbeg, len-1);
	    pret[i][len-1] = (char) 0;
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_INIT, "   argv[%d] = %s\n", i, pret[i]));

	    /* forward to next character */
	    pcur ++;

	    /* fetch next token */
	    continue;
	}

	/* shouldn't get here */
	assert (0);
    }
    /*}}}*/
#else /* !INTERNAL_ARGV_SPLIT */

    /*
     * Pass strings beginning with "!" but not "!!" to /bin/sh -c
     */
    if( *string == '!' && *(++string) != '!' )
    {
	/* Pass command to the shell for word splitting */
	pret[i++] = STRDUP( "/bin/sh" );
	pret[i++] = STRDUP( "-c" );
	pret[i++] = STRDUP( string );
    }

    else
    {
	/*
	 * Split command into words at spaces. White spaces can be quoted with
	 * a backslash. However we don't processes chains of "\\" which don't
	 * end in a space or tab. That is "\\a" expands to "\\a" however "\\ "
	 * expands to "\ ", and "\ " expands to a " " which does not split
	 * words.
	 */
	pcur = string;
	for( ; i < MAX_ARGV && *pcur; i++ )
	{
	    const int	max_argv_len = 1024;
	    char	argval[max_argv_len];
	    int		j = 0;

	    /* Skip leading spaces */
	    while( *pcur == ' ' || *pcur == '\t' ) pcur++;

	    while(
		    *pcur && *pcur != ' ' && *pcur != '\t' &&
		    j < max_argv_len - 1
		 )
	    {
		if( *pcur == '\\' )
		{
		    const char *s = pcur + 1;

		    /* Count backslashes */
		    while( *s == '\\' )
			s++;

		    if( *s == ' ' || *s == '\t' )
		    {
			int nbslashs = s - pcur;
			int nbytes = min( nbslashs / 2, max_argv_len - j - 1);

			/* Halve # backslashes */
			MEMSET( &argval[j], '\\', nbytes );
			pcur	=  s;
			j	+= nbytes;

			if( nbslashs % 2 == 0 )
			    break;  /* Split word here */
		    }

		    else
		    {
			/* Copy backslashes over verbatim */
			int nbytes = min( s - pcur, max_argv_len -j -1 );

			MEMCPY( &argval[j], pcur, nbytes );
			j	+= nbytes;
			pcur	=  s;
		    }
		} /* if( pcur = '\\' ) */

		argval[j++] = *pcur++;
	    } /* while( *pcur ... ) */

	    if( j )
	    {
		argval[j] = '\0';
		pret[i] = STRDUP( argval );
	    }
	    else
		break;
	}
    } /* else [ if( *string != '!' ) ] */
#endif /* !INTERNAL_ARGV_SPLIT */

#undef MAX_ARGV
    /* set the end of argv */
    if (pret[i])
    {
	*argc = i+1;
	SET_NULL(pret[i+1]);
    }
    else if (i)	    /* non-empty argv */
    {
	*argc = i;
    }
    else	    /* empty argv */
    {
#if 0
	/* 2006-02-23 gi1242: Also need to free the char* pointers in pret? */
	rxvt_free (pret);
	return NULL;
#endif
	goto NotMatch;
    }

    /* 2006-02-23  gi1242: Can now reduce the size of pret. */
    pret = (char **) rxvt_realloc( pret, (*argc + 1) * sizeof(char*) );
    return pret;

NotMatch:
    *argc = 0;
    {
	char **s;
	for( s = pret; NOT_NULL(*s); s++) rxvt_free(*s);
    }

    rxvt_free (pret);
    return NULL;
}