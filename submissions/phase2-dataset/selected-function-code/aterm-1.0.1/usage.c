usage(int type)
{
    int             i, col;

    fprintf(stderr, "\nUsage v%s :", VERSION);
    list_options();
    fprintf(stderr, "\n%s", APL_NAME);

    switch (type) {
    case 0:			/* brief listing */
	fprintf(stderr, " [-help][-V]\n");
	col = 3;
	for (i = 0; i < optList_size(); i++) {
	    if (optList[i].desc != NULL) {
		int             len = 2;

		if (!optList_isBool(i)) {
		    len = optList_strlen(i);
		    if (len > 0)
			len++;	/* account for space */
		}
		len += 4 + strlen(optList[i].opt);

		col += len;
		if (col > 79) {	/* assume regular width */
		    fprintf(stderr, "\n");
		    col = 3 + len;
		}
		fprintf(stderr, " [-");
		if (optList_isBool(i))
		    fprintf(stderr, "/+");
		fprintf(stderr, "%s", optList[i].opt);
		if (optList_strlen(i))
		    fprintf(stderr, " %s]", optList[i].arg);
		else
		    fprintf(stderr, "]");
	    }
	}
	fprintf(stderr, "\n\n");
	break;

    case 1:			/* full command-line listing */
	fprintf(stderr,
		" [options] [-e command args]\n\n"
		"where options include:\n");

	for (i = 0; i < optList_size(); i++)
	    if (optList[i].desc != NULL)
		fprintf(stderr, "    %s%s %-*s%s%s\n",
			(optList_isBool(i) ? "-/+" : "-"),
			optList[i].opt,
			(INDENT - strlen(optList[i].opt)
			 + (optList_isBool(i) ? 0 : 2)),
			(optList[i].arg ? optList[i].arg : ""),
			(optList_isBool(i) ? "turn on/off " : ""),
			optList[i].desc);
	fprintf(stderr, "\n    --help to list long-options\n    --version for the version information\n\n");
	break;

    case 2:			/* full resource listing */
	fprintf(stderr,
		" [options] [-e command args]\n\n"
		"where resources (long-options) include:\n");

	for (i = 0; i < optList_size(); i++)
	    if (optList[i].kw != NULL)
		fprintf(stderr, "    %s: %*s\n",
			optList[i].kw,
			(INDENT - strlen(optList[i].kw)),
			(optList_isBool(i) ? "boolean" : optList[i].arg));

#ifdef KEYSYM_RESOURCE
	fprintf(stderr, "    " "keysym.sym" ": %*s\n",
		(INDENT - strlen("keysym.sym")), "keysym");
#endif
	fprintf(stderr, "\n    -help to list options\n    -version for the version information with options list\n\n");
	break;
    }
    exit(EXIT_FAILURE);
}
