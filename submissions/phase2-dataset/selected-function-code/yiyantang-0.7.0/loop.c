{
	char ibuf[BUFFSIZE];
	char *obuf;
	char pbuf[BUFFSIZE + PRESIZE];
	int ret;
	fd_set fds;
	int max = 0;
	struct timeval tv;
	yytconfig curconfig;
	keystate inputstate;

#ifdef DEBUG
	int fd;
	fd = creat ("tty.log", S_IRUSR|S_IWUSR);
#endif

	curconfig.fromcode = fromcode;
	curconfig.tocode = tocode;
	curconfig.yytesc = YYTESC;
	curconfig.isnewbuf = TRUE;
	inputstate.altstate = 0;
	inputstate.cmdstate = 0;

	obuf = pbuf + PRESIZE;

	for (;;)
	{
		/* These will be reset if select returns -1. 
		 * tv need to be set in any cases. */
		tv.tv_sec = 10;
		tv.tv_usec = 100000;
		FD_ZERO (&fds);
		FD_SET (STDIN_FILENO, &fds);
		FD_SET (ptym, &fds);

		if (ptym > max)
			max = ptym;
		ret = select (max + 1, &fds, NULL, NULL, &tv);

		if (ret == 0 || (ret < 0 && errno == EINTR))
			continue;

		if (ret < 0)
		{
			/* Other errors are not pretty. */
			perror ("Select in loop.c");
			exit (ret);
		}

		/* Handle Standard Input to master tty. */
		if (FD_ISSET (STDIN_FILENO, &fds))
		{
			ret = read (STDIN_FILENO, ibuf, BUFFSIZE);
			if (ret > 0)
			{
				/* FIXME: Should this be TRUE or FALSE? 
				 * Maybe we want to reset half-hanzi for every
				 * input. 
				 */
				curconfig.isnewbuf = TRUE;
				parse_input(ibuf, &ret, &curconfig, &inputstate);
				if(curconfig.fromcode == -1)
				{
					/* Reset to initial state. */
					curconfig.tocode = tocode;
					curconfig.fromcode = fromcode;
				}
				if((curconfig.fromcode > 0) && curconfig.tocode)
				{
					/* For input, convert local encode to remote encode. */
					zhconv (curconfig, ibuf, &ret);
				}
				write (ptym, ibuf, ret);
			}
			else
				break;
		}

		/* Handle master tty to standard output. */
		if (FD_ISSET (ptym, &fds))
		{
			ret = read (ptym, obuf, BUFFSIZE);
			if (ret > 0)
			{
				char *tbuf1;
				char *tbuf2;
				char temp[BUFFSIZE + PRESIZE];
				int count = ret;

				tbuf1 = obuf;
				tbuf2 = temp + PRESIZE;

				curconfig.isnewbuf = TRUE;
#ifdef DEBUG
					write (fd, "\n\nEndOfBuf\n\n", 12);
#endif

				while (count > 0)
				{
					/* Don't need segment if encoding is already known. */
					if(curconfig.fromcode)
					{
						ret = count;
					}
					else
					{
						ret = hzsegment (tbuf1, count);
					}
					/* Operate on temp. */
					strncpy (tbuf2, tbuf1, ret);
#ifdef DEBUG
					write (fd, tbuf2, ret);
					write (fd, "\n\nEndOfHzB\n\n", 12);
#endif
					count -= ret;
					tbuf1 += ret;
					zhconv (curconfig, tbuf2, &ret);
					write (STDOUT_FILENO, tbuf2, ret);
					curconfig.isnewbuf = FALSE;
				}
			}
			else
				break;
		}
	}
}
