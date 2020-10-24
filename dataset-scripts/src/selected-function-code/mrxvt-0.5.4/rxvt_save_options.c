int
rxvt_save_options (rxvt_t* r, const char* filename)
{
    int		i;
    FILE*	pf = fopen (filename, "w");

    const char	*name = r->h->rs[Rs_name] ? r->h->rs[Rs_name] : APL_NAME;

    char	*tabs="\t\t\t\t\t";

    if (IS_NULL(pf))
	return 0;
    
    for (i = 0; i < optList_size(); i ++)
    {
	if(
	     IS_NULL( optList[i].kw ) ||	/* Options without a keyword
						   can't be written */
	     ( optList[i].doff == -1 && !optList_isBool(i) )
						/* Boolean options don't require
						 * a data offset. */
	  )
	    continue;

	if( optList[i].multiple)
	{
	    register int    j;

	    for (j = 0; j < MAX_PROFILES; j ++)
	    {
		if (r->h->rs[optList[i].doff + j])
		    fprintf( pf, "%s.profile%d.%s:%.*s%s\n", name,
			j, optList[i].kw,
			num_tabs( STRLEN(name) + sizeof(".profile") + 2
				    + STRLEN( optList[i].kw ) ), tabs,
			r->h->rs[optList[i].doff +j]);
	    }
	}
	else if (optList_isBool(i))
	{
	    int	    bval;
	    char*   OnOff[2] = {"False", "True"};

	    bval = ISSET_OPTION(r, optList[i].flag) ? 1 : 0;

	    if (optList_isReverse(i))
		bval = !bval;
	    fprintf( pf, "%s.%s:%.*s%s\n", name,
		optList[i].kw,
		num_tabs( STRLEN(name) + 1 + STRLEN(optList[i].kw) + 1 ), tabs,
		OnOff[bval] );
	}
	else if (r->h->rs[optList[i].doff])
	{
	    fprintf( pf, "%s.%s:%.*s%s\n", name,
		optList[i].kw,
		num_tabs( STRLEN(name) + 1 + STRLEN(optList[i].kw) + 1 ), tabs,
		r->h->rs[optList[i].doff] );
	}
    }

    fputs( "\n\n# vim: set ft=mrxvtrc :\n", pf );

    fclose (pf);
    return 1;
}