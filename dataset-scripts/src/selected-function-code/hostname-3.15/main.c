int
main(int argc, char **argv)
{
	char *file = NULL, *name = NULL;
	enum type_t type = DEFAULT;
	int boot = 0;
	int o;

	static const struct option long_options[] =
	{
		{"domain", no_argument, 0, 'd'},
		{"boot", no_argument, 0, 'b'},
		{"file", required_argument, 0, 'F'},
		{"fqdn", no_argument, 0, 'f'},
		{"all-fqdns", no_argument, 0, 'A'},
		{"help", no_argument, 0, 'h'},
		{"long", no_argument, 0, 'f'},
		{"short", no_argument, 0, 's'},
		{"version", no_argument, 0, 'V'},
		{"alias", no_argument, 0, 'a'},
		{"ip-address", no_argument, 0, 'i'},
		{"all-ip-addresses", no_argument, 0, 'I'},
		{"nis", no_argument, 0, 'y'},
		{"yp", no_argument, 0, 'y'},
		{0, 0, 0, 0}
	};

	/* If called as 'dnsdomainname', by default show the DNS domain name. */
	progname = (rindex(argv[0], '/')) ? rindex(argv[0], '/') + 1 : argv[0];
	if (!strcmp(progname, "dnsdomainname"))
		type = DNS;
	else if (!strcmp(progname, "domainname"))
		type = NIS;
	else if (!strcmp(progname, "ypdomainname"))
		type = NIS_DEF;
	else if (!strcmp(progname, "nisdomainname"))
		type = NIS_DEF;

	while((o = getopt_long(argc, argv, "aAdfbF:h?iIsVy", long_options, NULL)) != -1)
		switch (o) {
		case 'd':
			type = DNS;
			break;
		case 'a':
			type = ALIAS;
			break;
		case 'f':
			type = FQDN;
			break;
		case 'A':
			type = ALL_FQDNS;
			break;
		case 'i':
			type = IP;
			break;
		case 'I':
			type = ALL_IPS;
			break;
		case 's':
			type = SHORT;
			break;
		case 'y':
			type = NIS_DEF;
			break;
		case 'b':
			boot = 1;
			break;
		case 'F':
			file = optarg;
			break;
		case 'V':
			printf("hostname %s\n", VERSION);
			return 0;
		case '?':
		case 'h':
			usage(stdout);
			break;
		default:
			usage(stderr);
		}

	/*
	 * Hostname may be read from a file, it's ok for this file to not
	 * exist or be empty if -b is given in which case we default to
	 * "localhost" if there is not one already set.
	 */
	if (file) {
		name = read_file(file, boot);
		if (boot && (name == NULL || name[0] == '\0')) {
			free(name);

			name = localhost();
			if (name[0] == '\0' || !strcmp(name,"(none)"))
				strcpy(name, "localhost");
		}
	}

	/* Otherwise the hostname is the first non-option argument. */
	if (optind < argc) {
		/*
		 * It is an error to specify a host name as an argument, and to
		 * be read from a file.
		 */
		if (name)
			usage(stderr);

		if ((name = strdup(argv[optind])) == NULL)
			err(1, NULL);
		optind++;
	}

	/* Check for any remaining arguments. */
	if (optind < argc)
		usage(stderr);

	if (name) {
		set_name(type, name);
		free(name);
	} else
		show_name(type);

	return 0;
}