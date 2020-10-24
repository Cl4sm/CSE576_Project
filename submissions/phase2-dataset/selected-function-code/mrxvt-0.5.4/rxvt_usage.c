void
rxvt_usage(int type)
{
    unsigned int    i, col;

    write(STDOUT_FILENO, releasestring, sizeof(releasestring) - 1);
    write(STDOUT_FILENO, optionsstring, sizeof(optionsstring) - 1);
    write(STDOUT_FILENO, APL_NAME, sizeof(APL_NAME) - 1);

    switch (type)
    {
	case 0:		/* brief listing */
	    fprintf(stdout, " [-help] [--help]\n");
	    for (col = 1, i = 0; i < optList_size(); i++)
		if (NOT_NULL(optList[i].desc))
		{
		    int		 len = 0;

		    if (!optList_isBool(i))
		    {
			len = optList_STRLEN(i);
			if (len > 0)
			    len++;  /* account for space */
		    }
		    assert(NOT_NULL(optList[i].opt));

		    len += 4 + STRLEN(optList[i].opt) +
			(optList_isBool(i) ? 2: 0);
		    col += len;
		    if (col > 79)	    /* assume regular width */
		    {
			putc('\n', stdout);
			col = 1 + len;
		    }
		    fprintf(stdout, " [-%s%s", (optList_isBool(i) ?
			"/+" : ""), optList[i].opt);
		    if (optList_STRLEN(i))
			fprintf(stdout, " %s]", optList[i].arg);
		    else
			fprintf(stdout, "]");
		}
	    break;

	case 1:		/* full command-line listing */
	    fprintf(stdout, " [options] [-e command args]\n\n"
		"where options include:\n");
	    for (i = 0; i < optList_size(); i++)
		if (NOT_NULL(optList[i].desc))
		{
		    assert(NOT_NULL(optList[i].opt));

		    fprintf(stdout, "  %s%s %-*s%s%s\n",
			(optList_isBool(i) ? "-/+" : "-"), optList[i].opt,
			(INDENT - STRLEN(optList[i].opt)
			 + (optList_isBool(i) ? 0 : 2)),
			(optList[i].arg ? optList[i].arg : ""),
			(optList_isBool(i) ? "turn on/off " : ""),
			optList[i].desc);
		}
	    fprintf(stdout, "\n  --help to list long-options");
	    break;

	case 2:		/* full resource listing */
	    fprintf(stdout,
		" [options] [-e command args]\n\n"
		"where resources (long-options) include:\n");

#ifdef DEBUG
	    /*
	     * Print short options, long options and descriptions so we can
	     * generate man page.
	     */
	    for (i = 0; i < optList_size(); i++)
		fprintf( stdout, "%3d. %s   %s	%s  %s	%d\n", i,
			optList[i].kw ? optList[i].kw : "NullKW",
			optList[i].opt ? optList[i].opt : "NullOpt",
			!optList_isBool(i) ? optList[i].arg : "boolean",
			optList[i].desc ? optList[i].desc : "NullDsc",
			optList[i].multiple);

#else
	    for (i = 0; i < optList_size(); i++)
		if (NOT_NULL(optList[i].kw))
		    fprintf(stdout, "  %s: %*s%s\n",
			optList[i].kw,
			(INDENT - STRLEN(optList[i].kw)), "", /* XXX */
			(optList_isBool(i) ? "boolean" : optList[i].arg));
#endif
	    fprintf(stdout, "\n  -help to list options");
	    break;
    }	/* switch */

    fprintf(stdout, "\n\n");
    exit(EXIT_FAILURE);
    /* NOTREACHED */
}