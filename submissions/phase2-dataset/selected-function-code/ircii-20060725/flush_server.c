flush_server()
{
	fd_set rd;
	struct timeval time_out;
	int	flushing = 1;
	int	des;
	int	old_timeout;
	u_char	buffer[BIG_BUFFER_SIZE];

	if ((des = server_list[from_server].read) == -1)
		return;
	time_out.tv_usec = 0;
	time_out.tv_sec = 1;
	old_timeout = dgets_timeout(1);
	while (flushing)
	{
		FD_ZERO(&rd);
		FD_SET(des, &rd);
		switch (new_select(&rd, (fd_set *) 0, &time_out))
		{
		case -1:
		case 0:
			flushing = 0;
			break;
		default:
			if (FD_ISSET(des, &rd))
			{
				if (0 == dgets(buffer, sizeof buffer, des,
						(u_char *) 0))
					flushing = 0;
				
			}
			break;
		}
	}
	/* make sure we've read a full line from server */
	FD_ZERO(&rd);
	FD_SET(des, &rd);
	if (new_select(&rd, (fd_set *) 0, &time_out) > 0)
		dgets(buffer, sizeof buffer, des, (u_char *) 0);
	(void) dgets_timeout(old_timeout);
}
