int argc;
char *argv[];
{
	progname = argv[0];

	/* Set up defaults */
	DefaultOptions();

	/* Look for environment variable overrides */
	EnvOptions();

	/* Look for command line overrides */
	CommandOptions (argc, argv);

	/* Initialize files, etc. */
	InitStuff();
	atexit(do_exit);

	/* Do reply packet? */
	if (strcmp (rep_file, DEF_REP_FILE) && DoReply() < 0) {
		fprintf (stderr, "%s: error sending replies\n", progname);
		return(-1);
	}

	install_signals();

	/* Do news? */
	if (do_news && DoNews() < 0) {
		fprintf (stderr, "%s: error getting news\n", progname);
		return(-1);
	}

	/* Mail? */
	if (do_mail && DoMail() < 0) {
		fprintf (stderr, "%s: error getting mail\n", progname);
		return(-1);
	}

	return CloseStuff();
}
