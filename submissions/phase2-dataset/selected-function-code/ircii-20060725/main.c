main(argc, argv, envp)
	int	argc;
	char	*argv[];
	char	*envp[];
{
	char	lbuf[BUFFER_SIZE];
	size_t	cnt;
	fd_set	rd;

	if (argc < 2)
	{
	    fprintf(stderr, "Usage: %s [program] [arguments to program]\n", argv[0]);
	    exit(1);
	}
	pid = open("/dev/tty", O_RDWR);
#ifdef HAVE_SETSID
	setsid();
#else
	ioctl(pid, TIOCNOTTY, 0);
#endif /* HAVE_SETSID */
	setup_master_slave();
	switch (pid = fork())
	{
	case -1:
		fprintf(stderr, "flush: Unable to fork process!\n");
		exit(1);
	case 0:
		dup2(slave, 0);
		dup2(slave, 1);
		dup2(slave, 2);
		close(master);
		setuid(getuid());
		setgid(getgid());
		execvp(argv[1], &(argv[1]));
		fprintf(stderr, "flush: Error exec'ing process!\n");
		exit(1);
		break;
	default:
		(void) MY_SIGNAL(SIGCHLD, death, 0);
		close(slave);
		while (1)
		{
			FD_ZERO(&rd);
			FD_SET(master, &rd);
			FD_SET(0, &rd);
			switch (select(NFDBITS, &rd, 0, 0, 0))
			{
			case -1:
			case 0:
				break;
			default:
				if (FD_ISSET(0, &rd))
				{
				    if ((cnt = read(0, lbuf, sizeof lbuf)) > 0)
					write(master, lbuf, cnt);
				    else
					death(0);
				}
				if (FD_ISSET(master, &rd))
				{
					if ((cnt = read(master, lbuf,
							sizeof lbuf)) > 0)
						write(1, lbuf, cnt);
					else
						death(0);
				}
			}
		}
		break;
	}
	return 0;
}
