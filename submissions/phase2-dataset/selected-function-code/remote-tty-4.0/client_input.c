static void
client_input(int fd) {
	int nchars, i, new, query;
	struct passwd *pw;
	u_short salt;
	char s[3];
	u_int f;

	if (!WhosOn[fd])
		return;
	WhosOn[fd]->lastInput = Now;

	/* read the fixed part of the ttyprot (everything but the array)
	 */
	if (TP_FIXED != (nchars = read(fd, &T, TP_FIXED))) {
		dprintf(stderr, "client_input: read=%d on fd%d: ", nchars, fd);
#ifdef DEBUG
		if (Debug) perror("read");
#endif
		close_client(fd);
		return;
	}
#ifdef DEBUG_not
	cat_v(stderr, &T, nchars);
#endif
	i = ntohs(T.i);
	query = ntohs(T.f) & TP_QUERY;
	f = ntohs(T.f) & TP_TYPEMASK;
#ifdef DEBUG
	if (Debug) {
		lprintf(stderr,
			"client_input: nchars=%d fd%d i=0x%x o='%c' f=0x%x\n",
			nchars, fd, i, query?'Q':' ', f);
	}
#endif
	switch (f) {
	case TP_DATA:
		if (!(nchars = tp_getdata(fd, &T))) {
			close_client(fd);
			break;
		}
		dprintf(stderr, "client_input: TP_DATA, nchars=%d\n", nchars);
		if (WhosOn[fd]->state != auth)
			break;
# if WANT_CLIENT_LOGGING
		if (LogF) {
			if (nchars != fwrite(T.c, sizeof(char), nchars, LogF)){
				perror("fwrite(LogF)");
			} else {
				LogDirty = TRUE;
			}
		}
# endif /*WANT_CLIENT_LOGGING*/
		nchars = write(Tty, T.c, nchars);
#ifdef DEBUG
		if (Debug > 2) {
			lprintf(stderr, "client_input: wrote to tty: \"");
			cat_v(stderr, (u_char *)&T.c, nchars);
			fputs("\"\n", stderr);
		}
#endif
		break;
	case TP_BREAK:
		if (WhosOn[fd]->state != auth)
			break;
		dprintf(stderr, "client_input: sending break\n");
		tcsendbreak(Tty, 0);
		tp_senddata(fd, (u_char *)"BREAK", 5, TP_NOTICE);
		if (LogF)
			fputs("[BREAK]", LogF);
		dprintf(stderr, "client_input: done sending break\n");
		break;
	case TP_BAUD:
		if (WhosOn[fd]->state != auth)
			break;
		if (query) {
			tp_sendctl(fd, TP_BAUD|TP_QUERY, Baud, NULL);
			break;
		}

		if (-1 == (new = find_baud(i))) {
			tp_sendctl(fd, TP_BAUD, 0, NULL);
		} else {
			if (set_baud(new) != -1) {
				Baud = i;
				install_ttyios(Tty, &Ttyios);
				if (LogF) {
					fprintf(LogF, "[baud now %d]", i);
				}
				tp_sendctl(fd, TP_BAUD, 1, NULL);
			} else {
				tp_sendctl(fd, TP_BAUD, 0, NULL);
			}
		}
		break;
	case TP_PARITY:
		if (!query) {
			if (!(nchars = tp_getdata(fd, &T))) {
				close_client(fd);
				break;
			}
			T.c[i] = '\0';		/* XXX */
		}
		if (WhosOn[fd]->state != auth)
			break;
		if (query) {
			tp_sendctl(fd, TP_PARITY|TP_QUERY,
				   strlen(Parity), (u_char *)Parity);
			break;
		}
		if (-1 == (new = find_parity((char *)T.c))) {
			tp_sendctl(fd, TP_PARITY, 0, NULL);
		} else {
			strcpy(ParityBuf, (char *)T.c);
			Parity = ParityBuf;
			set_parity(new);
			install_ttyios(Tty, &Ttyios);
			if (LogF) {
				fprintf(LogF, "[parity now %s]", (char*) T.c);
			}
			tp_sendctl(fd, TP_PARITY, 1, NULL);
		}
		break;
	case TP_WORDSIZE:
		if (WhosOn[fd]->state != auth)
			break;
		if (query) {
			tp_sendctl(fd, TP_WORDSIZE|TP_QUERY, Wordsize, NULL);
			break;
		}
		if (-1 == (new = find_wordsize(i))) {
			tp_sendctl(fd, TP_WORDSIZE, 0, NULL);
		} else {
			Wordsize = i;
			set_wordsize(new);
			install_ttyios(Tty, &Ttyios);
			if (LogF) {
				fprintf(LogF, "[wordsize now %d]", i);
			}
			tp_sendctl(fd, TP_WORDSIZE, 1, NULL);
		}
		break;
	case TP_WHOSON:
		if (!query) {
			if (!(nchars = tp_getdata(fd, &T))) {
				close_client(fd);
				break;
			}
			T.c[i] = '\0';		/* XXX */
		}
		if (WhosOn[fd]->state != auth)
			break;
		if (query) {
			int iwho;

			for (iwho = getdtablesize()-1;  iwho >= 0;  iwho--) {
				struct whoson *who = WhosOn[iwho];
				char data[TP_MAXVAR];
				int idle;

				if (!who)
					continue;
				idle = Now - who->lastInput;
				sprintf(data, "%s [%s] (idle %d sec%s)",
					who->who ?who->who :"undeclared",
					who->host ?who->host :"?",
					idle, (idle==1) ?"" :"s");
				tp_senddata(fd, (u_char *)data, strlen(data),
					    TP_NOTICE);
			}
			break;
		}
		if (WhosOn[fd]) {
			if (WhosOn[fd]->who)
				free(WhosOn[fd]->who);
			WhosOn[fd]->who = safe_strdup((char *)T.c);
		}
		{ /*local*/
			char buf[TP_MAXVAR];

			sprintf(buf, "%-*.*s connected\07", i, i, T.c);
			broadcast((u_char *)buf, strlen(buf), TP_NOTICE);
		}
		break;
	case TP_TAIL:
		if (WhosOn[fd]->state != auth)
			break;
		if (!query)
			break;
		if (!LogF)
			break;
		fflush(LogF);
		LogDirty = FALSE;
		if (ftell(LogF) < 1024L) {
			if (0 > fseek(LogF, 0, SEEK_SET))
				break;
		} else {
			if (0 > fseek(LogF, -1024, SEEK_END))
				break;
		}
		{ /*local*/
			char buf[TP_MAXVAR];
			int len, something = FALSE;

			while (0 < (len = fread(buf, sizeof(char), sizeof buf,
						LogF))) {
				if (!something) {
					tp_senddata(fd, (u_char*)"tail+", 5,
						    TP_NOTICE);
					something = TRUE;
				}
				tp_senddata(fd, (u_char*)buf, len, TP_DATA);
			}
			if (something) {
				tp_senddata(fd, (u_char*)"tail-", 5,
					    TP_NOTICE);
			}
		}
		break;
	case TP_VERSION:
		if (!query)
			break;
		tp_senddata(fd, (u_char*)Version, strlen(Version), TP_NOTICE);
		break;
	case TP_LOGIN:
		if (!(nchars = tp_getdata(fd, &T))) {
			close_client(fd);
			break;
		}
		T.c[i] = '\0';		/* XXX */
		if (query)
			break;
		if (WhosOn[fd]->state != wlogin)
			break;
		pw = getpwnam((char*)T.c);
		if (!pw) {
			char data[TP_MAXVAR];

			sprintf(data, "%s - no such user", T.c);
			tp_senddata(fd, (u_char*)data, strlen(data),
				    TP_NOTICE);
		} else if (!pw->pw_passwd[0]) {
			auth_ok(fd);
		} else {
			WhosOn[fd]->state = wpasswd;
			WhosOn[fd]->auth = safe_strdup(pw->pw_passwd);
			salt = WhosOn[fd]->auth[0]<<8 | WhosOn[fd]->auth[1];
			tp_sendctl(fd, TP_PASSWD|TP_QUERY, salt, NULL);
		}
		break;
	case TP_PASSWD:
		if (!(nchars = tp_getdata(fd, &T))) {
			close_client(fd);
			break;
		}
		T.c[i] = '\0';		/* XXX */
		if (query)
			break;
		if (WhosOn[fd]->state != wpasswd)
			break;
		strncpy(s, WhosOn[fd]->auth, 2);
		if (!strcmp((char*)T.c, WhosOn[fd]->auth)) {
			auth_ok(fd);
		} else {
			char data[TP_MAXVAR];

			sprintf(data, "login incorrect");
			if (++WhosOn[fd]->aux > MAX_AUTH_ATTEMPTS) {
				close_client(fd);
			} else {
				tp_senddata(fd, (u_char*)data, strlen(data),
					    TP_NOTICE);
				auth_needed(fd);
			}
		}
		break;
	default:
		dprintf(stderr, "client_input: bad T: f=0x%x\n", ntohs(T.f));
		break;
	}
}
