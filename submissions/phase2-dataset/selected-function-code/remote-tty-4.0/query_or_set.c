static void
query_or_set(int fd, int ch) {
	int set, new, save_nonblock;
	char buf[64];

	switch (ch) {
	case 'q':
		set = 0;
		break;
	case 's':
		set = 1;
		break;
	default:
		fputc('G'-'@', stderr);
		return;
	}

	fputs(set ?"~set " :"~query ", stderr);
	save_nonblock = tty_nonblock(fd, 0);

	switch (read(fd, buf, 1)) {
	case -1:
		perror("read");
		goto done;
	case 0:
		fprintf(stderr, "!read");
		fflush(stderr);
		goto done;
	default:
		break;
	}

	switch (buf[0]) {
	case '\n':
	case '\r':
	case 'a':
		fputs("(show all)\r\n", stderr);
		tp_sendctl(Serv, TP_BAUD|TP_QUERY, 0, NULL);
		tp_sendctl(Serv, TP_PARITY|TP_QUERY, 0, NULL);
		tp_sendctl(Serv, TP_WORDSIZE|TP_QUERY, 0, NULL);
		break;
	case 'b':
		if (!set) {
			fputs("\07\r\n", stderr);
		} else {
			fputs("baud ", stderr);
			install_ttyios(fd, &Ttyios_orig);
			fgets(buf, sizeof buf, stdin);
			if (buf[strlen(buf)-1] == '\n') {
				buf[strlen(buf)-1] = '\0';
			}
			if (!(new = atoi(buf))) {
				break;
			}
			tp_sendctl(Serv, TP_BAUD, new, NULL);
		}
		break;
	case 'p':
		if (!set) {
			fputs("\07\r\n", stderr);
		} else {
			fputs("parity ", stderr);
			install_ttyios(fd, &Ttyios_orig);
			fgets(buf, sizeof buf, stdin);
			if (buf[strlen(buf)-1] == '\n') {
				buf[strlen(buf)-1] = '\0';
			}
			tp_sendctl(Serv, TP_PARITY, strlen(buf),
				   (u_char *)buf);
		}
		break;
	case 'w':
		if (!set) {
			fputs("\07\r\n", stderr);
		} else {
			fputs("wordsize ", stderr);
			install_ttyios(fd, &Ttyios_orig);
			fgets(buf, sizeof buf, stdin);
			if (!(new = atoi(buf))) {
				break;
			}
			tp_sendctl(Serv, TP_WORDSIZE, new, NULL);
		}
		break;
	case 'T':
		if (set) {
			fputs("\07\r\n", stderr);
		} else {
		       	fputs("Tail\r\n", stderr);
			tp_sendctl(Serv, TP_TAIL|TP_QUERY, 0, NULL);
		}
		break;
	case 'W':
		if (set) {
			fputs("\07\r\n", stderr);
		} else {
		       	fputs("Whoson\r\n", stderr);
			tp_sendctl(Serv, TP_WHOSON|TP_QUERY, 0, NULL);
		}
		break;
	case 'V':
		if (set) {
			fputs("\07\r\n", stderr);
		} else {
			fputs("Version\r\n", stderr);
			tp_sendctl(Serv, TP_VERSION|TP_QUERY, 0, NULL);
			fprintf(stderr, "[%s (client)]\r\n", Version);
		}
		break;
	default:
		if (set)
			fputs("[all baud parity wordsize]\r\n",
			      stderr);
		else
			fputs("[all Whoson Tail Version]\r\n", stderr);
		break;
	}
 done:
	(void) tty_nonblock(fd, save_nonblock);
	return;
}
