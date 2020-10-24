get_options(int argc, char *argv[])
{
    int             i, bad_option = 0;
    static const char *const On = "ON";
    static const char *const Off = "OFF";

    for (i = 1; i < argc; i++) {
	int             entry, longopt = 0;
	const char     *flag;
	char           *opt = argv[i];

#ifdef DEBUG_RESOURCES
	fprintf(stderr, "argv[%d] = %s: ", i, argv[i]);
#endif
	if (*opt == '-') {
	    flag = On;
	    if (*++opt == '-')
		longopt = *opt++;	/* long option */
	} else if (*opt == '+') {
	    flag = Off;
	    if (*++opt == '+')
		longopt = *opt++;	/* long option */
	} else {
	    bad_option = 1;
	    print_error("bad option \"%s\"", opt);
	    continue;
	}

	if (!strcmp(opt, "help"))
	    usage(longopt ? 2 : 1);
	if (!strcmp(opt, "h"))
	    usage(0);
	if (!strcmp(opt, "version"))
	    version(longopt ? 2 : 1);
	if (!strcmp(opt, "V"))
	    version(0);

    /* feature: always try to match long-options */
	for (entry = 0; entry < optList_size(); entry++)
	    if ((optList[entry].kw && !strcmp(opt, optList[entry].kw)) ||
		(!longopt &&
		 optList[entry].opt && !strcmp(opt, optList[entry].opt)))
		break;

	if (entry < optList_size()) {
	    if (optList_isReverse(entry))
		flag = flag == On ? Off : On;
	    if (optList_strlen(entry)) {	/* string value */
		char           *str = argv[++i];

#ifdef DEBUG_RESOURCES
		fprintf(stderr, "string (%s,%s) = ",
			optList[entry].opt ? optList[entry].opt : "nil",
			optList[entry].kw ? optList[entry].kw : "nil");
#endif
		if (flag == On && str && optList[entry].dp) {
#ifdef DEBUG_RESOURCES
		    fprintf(stderr, "\"%s\"\n", str);
#endif
		    *(optList[entry].dp) = str;

		/* special cases are handled in main.c:main() to allow
		 * X resources to set these values before we settle for
		 * default values
		 */
		}
#ifdef DEBUG_RESOURCES
		else
		    fprintf(stderr, "???\n");
#endif
	    } else {		/* boolean value */
#ifdef DEBUG_RESOURCES
		fprintf(stderr, "boolean (%s,%s) = %s\n",
			optList[entry].opt, optList[entry].kw, flag);
#endif
		if (flag == On)
		    Options |= (optList[entry].flag);
		else
		    Options &= ~(optList[entry].flag);

		if (optList[entry].dp)
		    *(optList[entry].dp) = flag;
	    }
	} else
#ifdef KEYSYM_RESOURCE
	/* if (!strncmp (opt, "keysym.", strlen ("keysym."))) */
	if (Str_match(opt, "keysym.")) {
	    char           *str = argv[++i];

	/*
	 * '7' is strlen("keysym.")
	 */
	    if (str != NULL)
		parse_keysym(opt + 7, str);
	} else
#endif
	{
	/* various old-style options, just ignore
	 * Obsolete since about Jan 96,
	 * so they can probably eventually be removed
	 */
	    const char     *msg = "bad";

	    if (longopt) {
		opt--;
		bad_option = 1;
	    } else if (!strcmp(opt, "7") || !strcmp(opt, "8")
#ifdef GREEK_SUPPORT
	    /* obsolete 12 May 1996 (v2.17) */
		       || !Str_match(opt, "grk")
#endif
		)
		msg = "obsolete";
	    else
		bad_option = 1;

	    print_error("%s option \"%s\"", msg, --opt);
	}
    }

    if (bad_option)
	usage(0);
}
