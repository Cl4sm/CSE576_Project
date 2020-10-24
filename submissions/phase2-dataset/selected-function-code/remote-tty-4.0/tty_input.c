tty_input(int fd) {
	static enum {base, need_cr, tilde} state = base;
	u_char buf[1];
	int n, save_nonblock;

	save_nonblock = tty_nonblock(fd, 1);
	while ((n = read(fd, buf, 1)) == 1) {
		u_char ch = buf[0];

		switch (state) {
		case base:
			if (ch == '~') {
				state = tilde;
				continue;
			}
			if (ch != '\r') {
				state = need_cr;
			}
			break;
		case need_cr:
			/* \04 (^D) is a line terminator on some systems */
			if (ch == '\r' || ch == '\04') {
				state = base;
			}
			break;
		case tilde:
#define RESTRICTED_HELP_STR "\r\n\
~^Z - suspend program\r\n\
~^L - set logging\r\n\
~q  - query server\r\n\
~s  - set option\r\n\
"
#define UNRESTRICTED_HELP_STR "\r\n\
~~  - send one tilde (~)\r\n\
~.  - exit program\r\n\
~#  - send BREAK\r\n\
~?  - this message\r\n\
"
			state = base;
			switch (ch) {
			case '~': /* ~~ - write one ~, which is in buf[] */
				break;
			case '.': /* ~. - quitsville */
				(void) tty_nonblock(fd, 0);
				quit(0);
				/* FALLTHROUGH */
			case 'L'-'@': /* ~^L - start logging */
				if (Restricted)
					goto passthrough;
				install_ttyios(fd, &Ttyios_orig);
				logging(fd);
				install_ttyios(fd, &Ttyios);
				continue;
			case 'Z'-'@': /* ~^Z - suspend yourself */
				if (Restricted)
					goto passthrough;
				install_ttyios(fd, &Ttyios_orig);
				kill(getpid(), SIGTSTP);
				install_ttyios(fd, &Ttyios);
				continue;
			case 'q': /* ~q - query server */
				/*FALLTHROUGH*/
			case 's': /* ~s - set option */
				if (Restricted)
					goto passthrough;
				query_or_set(fd, ch);
				continue;
			case '#': /* ~# - send break */
				tp_sendctl(Serv, TP_BREAK, 0, NULL);
				continue;
			case '?': /* ~? - help */
				if (!Restricted)
					fprintf(stderr, RESTRICTED_HELP_STR);
				fprintf(stderr, UNRESTRICTED_HELP_STR);
				continue;
 passthrough:
			default: /* ~mumble - write; `mumble' is in buf[] */
				tp_senddata(Serv, (u_char *)"~", 1,
					    TP_DATA);
				if (Log != -1) {
					write(Log, "~", 1);
				}
				break;
			}
			break;
		}
		if (0 > tp_senddata(Serv, buf, 1, TP_DATA)) {
			server_died();
		}
		if (Log != -1) {
			write(Log, buf, 1);
		}
	}
	(void) tty_nonblock(fd, save_nonblock);
	if (n == 0)
		quit(0);
}
