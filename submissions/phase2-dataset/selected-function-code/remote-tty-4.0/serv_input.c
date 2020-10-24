serv_input(int fd) {
	char passwd[TP_MAXVAR], s[3], *c, *crypt();
	int nchars, i;
	u_int f, o, t;

	if (0 >= (nchars = read(fd, &T, TP_FIXED))) {
		fprintf(stderr, "serv_input: read(%d) returns %d\n",
			fd, nchars);
		server_died();
	}

	i = ntohs(T.i);
	f = ntohs(T.f);
	o = f & TP_OPTIONMASK;
	t = f & TP_TYPEMASK;
	switch (t) {
	case TP_DATA:	/* FALLTHROUGH */
	case TP_NOTICE:
		if (i != (nchars = read(fd, T.c, i))) {
			fprintf(stderr, "serv_input: read@%d need %d got %d\n",
				fd, i, nchars);
			server_died();
		}
		if (SevenBit) {
			int i;

			for (i = 0; i < nchars; i++)
				T.c[i] &= 0x7f;
		}
		switch (t) {
		case TP_DATA:
			write(STDOUT_FILENO, T.c, nchars);
			if (Log != -1)
				write(Log, T.c, nchars);
			break;
		case TP_NOTICE:
			write(STDOUT_FILENO, "[", 1);
			write(STDOUT_FILENO, T.c, nchars);
			write(STDOUT_FILENO, "]\r\n", 3);
			if (Log != -1) {
				write(Log, "[", 1);
				write(Log, T.c, nchars);
				write(Log, "]\r\n", 3);
			}
			break;
		default:
			break;
		}
		break;
	case TP_BAUD:
		if ((o & TP_QUERY) != 0)
			fprintf(stderr, "[baud %d]\r\n", i);
		else
			server_replied("baud rate change", i);
		break;
	case TP_PARITY:
		if ((o & TP_QUERY) != 0) {
			if (i != (nchars = read(fd, T.c, i))) {
				server_died();
			}
			T.c[i] = '\0';
			fprintf(stderr, "[parity %s]\r\n", T.c);
		} else {
			server_replied("parity change", i);
		}
		break;
	case TP_WORDSIZE:
		if ((o & TP_QUERY) != 0)
			fprintf(stderr, "[wordsize %d]\r\n", i);
		else
			server_replied("wordsize change", i);
		break;
	case TP_LOGIN:
		if ((o & TP_QUERY) == 0)
			break;
		tp_sendctl(Serv, TP_LOGIN, strlen(Login), (u_char*)Login);
		break;
	case TP_PASSWD:
		if ((o & TP_QUERY) == 0)
			break;
		fputs("Password:", stderr); fflush(stderr);
		for (c = passwd;  c < &passwd[sizeof passwd];  c++) {
			fd_set infd;

			FD_ZERO(&infd);
			FD_SET(Tty, &infd);
			if (1 != select(Tty+1, &infd, NULL, NULL, NULL))
				break;
			if (1 != read(Tty, c, 1))
			       	break;
			if (*c == '\r')
				break;
		}
		*c = '\0';
		fputs("\r\n", stderr); fflush(stderr);
		s[0] = 0xff & (i>>8);
		s[1] = 0xff & i;
		s[2] = '\0';
		c = crypt(passwd, s);
		tp_sendctl(Serv, TP_PASSWD, strlen(c), (u_char*)c);
		break;
	}
}
