main(int argc, char *argv[]) {
	int ch;

	ProgName = argv[0];

	if (!(Login = getlogin())) {
		struct passwd *pw = getpwuid(getuid());

		if (pw) {
			Login = pw->pw_name;
		} else {
			Login = "nobody";
		}
	}
	if (!(TtyName = ttyname(STDIN_FILENO))) {
		TtyName = "/dev/null";
	}

	while ((ch = getopt(argc, argv, "s:x:l:7r")) != EOF) {
		switch (ch) {
		case 's':
			ServSpec = optarg;
			break;
#ifdef DEBUG
		case 'x':
			Debug = atoi(optarg);
			break;
#endif
		case 'l':
			Login = optarg;
			break;
		case 'r':
			Restricted++;
			break;
		case '7':
			SevenBit++;
			break;
		default:
			USAGE((stderr, "%s: getopt=%c ?\n", ProgName, ch));
		}
	}

	if (optind != argc-1) {
		USAGE((stderr, "must specify service\n"));
	}
	ServSpec = argv[optind++];
	sprintf(WhoAmI, "%s@%s", Login, TtyName);

	if (ServSpec[0] == '/') {
		struct sockaddr_un n;

		Serv = socket(PF_UNIX, SOCK_STREAM, 0);
		ASSERT(Serv>=0, "socket");

		n.sun_family = AF_UNIX;
		(void) strcpy(n.sun_path, ServSpec);

		ASSERT(0<=connect(Serv, (struct sockaddr *)&n, sizeof n),
		       n.sun_path);
		dprintf(stderr, "rtty.main: connected on fd%d\r\n", Serv);
		fprintf(stderr, "connected\n");
	} else {
#ifdef WANT_TCPIP
		int loc, len;
		locbrok lb;
		char buf[10];
		char *cp = strchr(ServSpec, '@');

		if (cp)
			*cp++ = '\0';
		else
			cp = "127.0.0.1";

		if ((loc = rconnect(cp, "locbrok", NULL,stderr,30)) == -1) {
			fprintf(stderr, "can't contact location broker\n");
			quit(0);
		}
		lb.lb_port = htons(0);
		len = min(LB_MAXNAMELEN, strlen(ServSpec));
		lb.lb_nlen = htons(len);
		strncpy(lb.lb_name, ServSpec, len);
		ASSERT(write(loc, &lb, sizeof lb)==sizeof lb, "write lb");
		ASSERT(read(loc, &lb, sizeof lb)==sizeof lb, "read lb");
		close(loc);
		lb.lb_port = ntohs(lb.lb_port);
		dprintf(stderr, "(locbrok: port %d)\n", lb.lb_port);
		if (!lb.lb_port) {
			fprintf(stderr, "location broker can't find %s@%s\n",
				ServSpec, cp);
			quit(0);
		}
		sprintf(buf, "%d", lb.lb_port);
		Serv = rconnect(cp, buf, NULL,stderr,30);
		ASSERT(Serv >= 0, "rconnect rtty");
#else
		USAGE((stderr, "service must begin with a '/'\n"));
#endif /*WANT_TCPIP*/
	}

	tcgetattr(Tty, &Ttyios);
	Ttyios_orig = Ttyios;
	prepare_term(&Ttyios, 1);
	signal(SIGINT, quit);
	signal(SIGQUIT, quit);
	install_ttyios(Tty, &Ttyios);
	Ttyios_set++;

	fprintf(stderr,
		"(use (CR)~? for minimal help; also (CR)~q? and (CR)~s?)\r\n");
	tp_sendctl(Serv, TP_WHOSON, strlen(WhoAmI), (u_char*)WhoAmI);
	main_loop();
	exit(0);
}
