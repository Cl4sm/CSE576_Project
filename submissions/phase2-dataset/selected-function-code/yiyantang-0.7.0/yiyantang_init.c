{
	struct gengetopt_args_info args_info;
/*	int c;
 */
	char **cmd;

	*fromcode = 0;
	*tocode = gettocode ();
	cmd = getshell ();

	hz_setup();
#if 0
	opterr = 0;			/* getopt variable. */
	while ((c = getopt (argc, argv, "+f:t:")) != -1)
	{
		switch (c)
		{
			case 'f':
				parseencode (optarg, fromcode);
				break;
			case 't':
				parseencode (optarg, tocode);
				//printf ("tocode: %d\n", *tocode);
				break;
			case 'h':
			case '?':
				printusage ();
		}
	}
	
	if (optind < argc)
	{
		cmd = &argv[optind];
	}
#endif /* 0 */
	if(cmdline_parser(argc, argv, &args_info) != 0)
	{
		cmdline_parser_print_help();
		cmdline_parser_print_version();
		exit(1);
	}
	if(args_info.from_given)
		parseencode(args_info.from_arg, fromcode);
	if(args_info.to_given)
		parseencode(args_info.to_arg, tocode);
	if(args_info.inputs_num)
		cmd = args_info.inputs;

	return (cmd);

}
