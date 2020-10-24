static void
setup_master_slave()
{
	char	line[11];
	char	linec;
	int	linen;

	for (linec = 'p'; linec <= 's'; linec++)
	{
		snprintf(line, sizeof line, "/dev/pty%c0", linec);
		if (access(line, 0) != 0)
			break;
		for (linen = 0; linen < 16; linen++)
		{
			snprintf(line, sizeof line, "/dev/pty%c%1x", linec, linen);
			if ((master = open(line, O_RDWR)) >= 0)
			{
				snprintf(line, sizeof line, "/dev/tty%c%1x", linec, linen);
				if (access(line, R_OK | W_OK) == 0)
				{
					if ((slave = open(line, O_RDWR)) >= 0)
						return;
				}
				close(master);
			}
		}
	}
	fprintf(stderr, "flush: Can't find a pty\n");
	exit(0);
}
