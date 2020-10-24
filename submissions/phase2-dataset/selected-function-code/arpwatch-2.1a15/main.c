int
main(int argc, char **argv)
{
	register char *cp;
	register int op, pid, snaplen, timeout, linktype, status;
#ifdef TIOCNOTTY
	register int fd;
#endif
	register pcap_t *pd;
	register char *rfilename;
	struct bpf_program code;
	char errbuf[PCAP_ERRBUF_SIZE];
	char* username = NULL;
	int restart = 0;
	char options[] =
		"d"
		/**/
		/**/
		"f:"
		/**/
		/**/
		"i:"
		/**/
		/**/
		"n:"
		/**/
		/**/
		"N"
		/**/
		/**/
		"r:"
		/**/
		/**/
		"s:"
		/**/
		/**/
		"p"
		/**/
		/**/
		"a"
		/**/
		/**/
		"m:"
		/**/
		/**/
		"u:"
		"R:"
		/**/
		/**/
		"Q"
		/**/
		/**/
		"z:"
		/**/
		/**/
	;
	char *tmpptr;

	if (argv[0] == NULL)
		prog = "arpwatch";
	else if ((cp = strrchr(argv[0], '/')) != NULL)
		prog = cp + 1;
	else
		prog = argv[0];

	if (abort_on_misalignment(errbuf) < 0) {
		(void)fprintf(stderr, "%s: %s\n", prog, errbuf);
		exit(1);
	}

	opterr = 0;
	interface = NULL;
	rfilename = NULL;
	pd = NULL;

	inet_aton("0.0.0.0", &ignore_netmask);
	inet_aton("255.255.255.255", &ignore_netmask);
	while ((op = getopt(argc, argv, options)) != EOF)
		switch (op) {

		case 'a':
			++allsubnets;
			break;

		case 'd':
			++debug;
#ifndef DEBUG
			(void)fprintf(stderr,
			    "%s: Warning: Not compiled with -DDEBUG\n", prog);
#endif
			break;

		case 'f':
			arpfile = optarg;
			break;

		case 'i':
			interface = optarg;
			break;

		case 'n':
			if (!addnet(optarg))
				usage();
			break;

		case 'N':
			++nobogons;
			break;

		case 'r':
			rfilename = optarg;
			break;
		/**/
		/**/
		case 's':
			path_sendmail = optarg;
			break;
		/**/
		/**/
		case 'p':
			++nopromisc;
			break;
		/**/
		/**/
		case 'm':
			mailaddress = optarg;
			break;
		/**/
		/**/
		case 'u':
			if ( optarg ) {
				username = strdup(optarg);
			} else {
				fprintf(stderr, "%s: Need username after -u\n", prog);
				usage();
			}
			break;
		case 'R':
			restart = atoi(optarg);
			break;
		/**/
		/**/
		case 'Q':
		        ++quiet;
			break;

		/**/
		/**/
		case 'z':
			tmpptr = strtok(optarg, "/");
			inet_aton(tmpptr, &ignore_net);
			tmpptr = strtok(NULL, "/");
			inet_aton(tmpptr, &ignore_netmask);
			break;
		/**/
		/**/
		default:
			usage();
		}

	if (optind != argc)
		usage();

	if (rfilename != NULL) {
		net = 0;
		netmask = 0;
		interface = "(from file)";
		restart = 0;
	} else {
		/* Determine interface if not specified */
		if (interface == NULL &&
		    (interface = pcap_lookupdev(errbuf)) == NULL) {
			(void)fprintf(stderr, "%s: lookup_device: %s\n",
			    prog, errbuf);
			exit(1);
		}

		/* Determine network and netmask */
		if (pcap_lookupnet(interface, &net, &netmask, errbuf) < 0) {
			syslog(LOG_NOTICE, "bad interface %s: %s - assuming unconfigured interface",
				interface, errbuf);
			net = 0;
			netmask = 0;
		}

		/* Drop into the background if not debugging */
		if (!debug) {
			pid = fork();
			if (pid < 0) {
				syslog(LOG_ERR, "main fork(): %m");
				exit(1);
			} else if (pid != 0)
				exit(0);
			(void)close(fileno(stdin));
			(void)close(fileno(stdout));
			(void)close(fileno(stderr));
#ifdef TIOCNOTTY
			fd = open("/dev/tty", O_RDWR);
			if (fd >= 0) {
				(void)ioctl(fd, TIOCNOTTY, 0);
				(void)close(fd);
			}
#else
			(void) setsid();
#endif
		}
	}

	openlog(prog, 0, LOG_DAEMON);

	if (chdir(arpdir) < 0) {
		syslog(LOG_ERR, "chdir(%s): %m", arpdir);
		syslog(LOG_ERR, "(using current working directory)");
	}

label_restart:
	if (rfilename != NULL) {
		pd = pcap_open_offline(rfilename, errbuf);
		if (pd == NULL) {
			syslog(LOG_ERR, "pcap open %s: %s", rfilename,  errbuf);
			exit(1);
		}
		swapped = pcap_is_swapped(pd);
	} else {
		snaplen = max(sizeof(struct ether_header),
		    sizeof(struct fddi_header)) + sizeof(struct ether_arp);
		timeout = 1000;
		pd = pcap_open_live(interface, snaplen, !nopromisc, timeout, errbuf);
		if (pd == NULL) {
			syslog(LOG_ERR, "pcap open %s: %s", interface, errbuf);
			if (restart) {
				syslog(LOG_ERR, "restart in %d secs", restart);
			} else {
				exit(1);
			}
			sleep(restart);
			goto label_restart;
		}
#ifdef WORDS_BIGENDIAN
		swapped = 1;
#endif
	}

        if ( username && !restart ) {
               dropprivileges( username );
        } else {
	/*
	 * Revert to non-privileged user after opening sockets
	 * (not needed on most systems).
	 */
		setgid(getgid());
		setuid(getuid());
	}

	/* Must be ethernet or fddi */
	linktype = pcap_datalink(pd);
	if (linktype != DLT_EN10MB && linktype != DLT_FDDI) {
		syslog(LOG_ERR, "Link layer type %d not ethernet or fddi",
		    linktype);
		exit(1);
	}

	/* Compile and install filter */
	if (pcap_compile(pd, &code, "arp or rarp", 1, netmask) < 0) {
		syslog(LOG_ERR, "pcap_compile: %s", pcap_geterr(pd));
		exit(1);
	}
	if (pcap_setfilter(pd, &code) < 0) {
		syslog(LOG_ERR, "pcap_setfilter: %s", pcap_geterr(pd));
		exit(1);
	}
	if (rfilename == NULL)
		syslog(LOG_INFO, "listening on %s", interface);

	/* Read in database */
	initializing = 1;
	if (!readdata())
		exit(1);
	sorteinfo();
#ifdef DEBUG
	if (debug > 2) {
		debugdump();
		exit(0);
	}
#endif
	initializing = 0;

	(void)setsignal(SIGINT, die);
	(void)setsignal(SIGTERM, die);
	(void)setsignal(SIGHUP, die);
	if (rfilename == NULL) {
		(void)setsignal(SIGQUIT, checkpoint);
		(void)setsignal(SIGALRM, checkpoint);
		(void)alarm(CHECKPOINT);
	}

	switch (linktype) {

	case DLT_EN10MB:
		status = pcap_loop(pd, 0, process_ether, NULL);
		break;

	case DLT_FDDI:
		status = pcap_loop(pd, 0, process_fddi, NULL);
		break;

	default:
		syslog(LOG_ERR, "bad linktype %d (can't happen)", linktype);
		exit(1);
	}
	if (status < 0) {
		syslog(LOG_ERR, "pcap_loop: %s", pcap_geterr(pd));
		exit(1);
	}
	pcap_close(pd);
	if (!dump())
		exit(1);
	exit(0);
}