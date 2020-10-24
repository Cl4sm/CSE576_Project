int main (argc,argv)
     int argc;
     char ** argv;
     
{
    set_progname(argv[0]);

#if 0
    if (sizeof(Nshort) != 2) {
	fprintf(stderr, "%s: greivous porting error. sizeof(Nshort) == %d, not 2.\n",
		progname, sizeof(Nshort));
	exit(EXITCODE_ARGS);
    }
#endif

    while (argc>1) {
	char *arg = argv[1];
	if (arg[0] != '-') {
	    break;
	}
	arg++;
	if (arg[0] == '-') {
	    arg++;
	    if (0==strcmp(arg, "verbose")) {
		verbose = 1;
		argv++; argc--;
	    } else if (0==strcmp(arg, "fd")) {
		argv++; argc--;
		if (argc<2) {
		    fprintf(stderr, "%s: --fd requires file number for socket.\n",
			    progname);
		    usage();
		    exit(EXITCODE_ARGS);
		} else if (sockfd>=0) {
		    fprintf(stderr, "%s: --fd can only be specified once\n",
			    progname);
		    usage();
		    exit(EXITCODE_ARGS);
		} else  {
		    sockfd = atoi(argv[1]);
		    argv++; argc--;
		}
	    } else if (0==strcmp(arg, "subproc")) {
		subproc = 1;
		argv++; argc--;
	    } else if (0==strcmp(arg, "infd") ||
		       0==strcmp(arg, "outfd") ||
		       0==strcmp(arg, "duplex") ||
		       0==strcmp(arg, "Duplex") ||
		       0==strcmp(arg, "DUPLEX")) {
		long	fd, sid;
		char	*p;
		int	err = 0;
		enum pipe_polarity	pol = -1;
		argv++; argc--;
		if (argc<2 || argv[1][0] == 0) {
		    err = 1;
		} else {
		    fd = strtol(argv[1], &p, 0);
		    if (*p==0) {
			sid = -1;
		    } else if (p[0] == '=' && p[1] != 0) {
			sid = strtol(p+1, &p, 0);
			if (*p != 0) {
			    err = 1;
			}
		    } else {
			err = 1;
		    }
		}
		if (err) {
		    fprintf(stderr, "%s: %s requires descriptor number as argument\n", progname, arg-2);
		    usage();
		    exit(EXITCODE_ARGS);
		}

		{
		  if      (0==strcmp(arg, "infd"))   pol = WRITEABLE;
		  else if (0==strcmp(arg, "outfd"))  pol = READABLE;
		  else if (0==strcmp(arg, "duplex")) pol = DUPLEX_IO;
		  else if (0==strcmp(arg, "Duplex")) pol = DUPLEX;
		  else if (0==strcmp(arg, "DUPLEX")) pol = DUPLEX_OI;
		}
		if (pol == -1) {
		    fprintf(stderr, "%s: code error, polarity uninitialized %s:%d\n", progname, __FILE__, __LINE__);
		    abort();
		}
		add_subproc_fd(fd, pol, -1);
		argv++; argc--;
	    } else if (0==strcmp(arg, "prefer-local")||
		       0==strcmp(arg, "preferlocal")) {
		merge_returns = PREFER_LOCAL;
		argv++; argc--;
	    } else if (0==strcmp(arg, "prefer-remote")||
		       0==strcmp(arg, "preferremote")) {
		merge_returns = PREFER_REMOTE;
		argv++; argc--;
	    } else if (0==strcmp(arg, "local-only")||
		       0==strcmp(arg, "localonly")) {
		merge_returns = LOCAL_ONLY;
		argv++; argc--;
	    } else if (0==strcmp(arg, "remote-only")||
		       0==strcmp(arg, "remoteonly")) {
		merge_returns = REMOTE_ONLY;
		argv++; argc--;
	    } else if (0==strcmp(arg, "client")) {
		im_server = 0;
		argv++; argc--;
	    } else if (0==strcmp(arg, "server")) {
		im_server = 1;
		argv++; argc--;
	    } else {
		/* unknown -- flag.  Assume it's a command :) */
		break;
	    }
	} else {
	    /* it's a set of single dash flags. */
	    do { switch (arg[0]) {
	    case '#':
		arg += scan_flag_numeric_fd(arg+1, &sockfd);
		break;
	    case 'v':
		verbose = 1;
		break;
	    case 's':
		subproc=1;
		break;
	    case 'i':
		if (arg[1] == 'o') {
		    int	fd;
		    arg += scan_flag_numeric_fd(arg+2, &fd);
		    add_subproc_fd(fd, DUPLEX_IO, -1);
		} else {
		    int	fd;
		    arg += scan_flag_numeric_fd(arg+1, &fd);
		    add_subproc_fd(fd, WRITEABLE, -1);
		}
		break;
	    case 'o':
		if (arg[1] == 'i') {
		    int	fd;
		    arg += scan_flag_numeric_fd(arg+2, &fd);
		    add_subproc_fd(fd, DUPLEX_OI, -1);
		} else {
		    int	fd;
		    arg += scan_flag_numeric_fd(arg+1, &fd);
		    add_subproc_fd(fd, READABLE, -1);
		}
		break;
	    case 'd':
		{
		    int	fd;
		    arg += scan_flag_numeric_fd(arg+1, &fd);
		    add_subproc_fd(fd, DUPLEX, -1);
		}
		break;
	    case 'l':
		merge_returns = PREFER_LOCAL;
		break;
	    case 'r':
		merge_returns = PREFER_REMOTE;
		break;
	    case 'L':
		merge_returns = LOCAL_ONLY;
		break;
	    case 'R':
		merge_returns = REMOTE_ONLY;
		break;
	    case 0:
		fprintf(stderr, "%s: blank compact flag.\n", progname);
		/* fall through */
	    default:
		fprintf(stderr, "%s: Unknown compact flag beginning %s\n", progname, arg);
		usage();
		exit (EXITCODE_ARGS);
	    } arg++;
	    } while (arg[0]);

	    argv++;
	    argc--;
	}
    }

    /* argv+1 points to an unrecognized flag that must be the
       subprocess cmd and arguments. */

    if (argc>1 && !subproc) {
	fprintf(stderr, "%s: Unknown flag %s\n", progname, argv[1]);
	usage();
	exit (EXITCODE_ARGS);
    }

    if (sockfd<0) {
	fprintf(stderr, "%s:  I must know the file number for the socket.\n",
		progname);
	usage();
	exit(EXITCODE_ARGS);
    }

    if (subproc) {
	if (merge_returns == UNINITIALIZED)
	    merge_returns = PREFER_LOCAL;
	/* check to make sure at least one descriptor is rerouted */
	if (pipe_head == 0) {
	    fprintf(stderr, "%s: must redirect at least one descriptor of subprocess.\n", progname);
	    usage();
	    exit(EXITCODE_ARGS);
	}
    } else {
	if (pipe_head == 0) {
	    /* rig the degenerate case */
	    add_subproc_fd(0, WRITEABLE, -1);
	    add_subproc_fd(1, READABLE, -1);
	}
	if (merge_returns == PREFER_LOCAL ||
	    merge_returns == PREFER_REMOTE ||
	    merge_returns == LOCAL_ONLY ||
	    merge_returns == REMOTE_ONLY) {
	    fprintf(stderr, "%s: no local process to get a return code from\n", progname);
	    usage();
	    exit (EXITCODE_ARGS);
	}

	merge_returns = LOCAL_ONLY;
    }

    add_control_channel();	/* for passing exit status.  Is DUPLEX_OI. */

    if (verbose)
        emit_version("encapsulate", 1996);

    if (im_server<0) {
	im_server = Im_server_p(sockfd);
    }

    {
	struct pipeline	*curr;
	for (curr = pipe_head; curr; curr = curr->next) {
	    if (curr->code == DUPLEX)
		curr->code = im_server ? DUPLEX_IO : DUPLEX_OI;
	}

	/* might as well initialize our session_id counter */
	session_id_ = im_server ? 1025 : 1024;
	/* below 1024 is reserved */
    }


    if (subproc)
	spawn_child(argv+1, sockfd);
    else {
	/* I have to invert the polarity of writable and readable
	   channels, but not duplex.  Also have to copy the child_fd
	   to pipe[0].  What a hellish mess. */
	rig_single(); }

    signal(SIGPIPE, SIG_IGN);	/* handle the error returns */

    {
	char	*buf;
	int	len;
	buf = prepare_SYNs( &len);
	prime_packet_queue(buf, len);
    }

    main_io_loop(sockfd);

#ifdef DEBUG
    fprintf(stderr, "%s: end of IO loop\n", progname);
#endif

#if 0
    while (child_running) {
	pause();
    }
    probe_child();
#endif

    if (local_return_code ==0)
	local_return_code = exitcode_warnings;

    switch (merge_returns) {
    case PREFER_LOCAL:
	if (local_return_code!=0)
	    exit (local_return_code);
	else
	    exit (remote_return_code);
	/* NOTREACHED */
    case PREFER_REMOTE:
	if (remote_return_code!=0)
	    exit (remote_return_code);
	else
	    exit (local_return_code);
	/* NOTREACHED */
    case LOCAL_ONLY:
	exit(local_return_code);
    case REMOTE_ONLY:
	exit(remote_return_code);
    default:
	fprintf(stderr, "%s: logic error.  merge_returns has bogus value.\n",
		progname);
	exit(EXITCODE_ARGS);
    }
}