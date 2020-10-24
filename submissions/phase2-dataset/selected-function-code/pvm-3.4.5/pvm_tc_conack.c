static int
pvm_tc_conack(mid)
	int mid;
{
	int src;					/* sender of reply */
	int ttpro;					/* protocol revision */
	int ackd;					/* connection allowed (0) */
	struct ttpcb *pcbp;			/* pcb for connection */
	static int linger[2] = { 1, 60 };   /* XXX arbitrary time */
	int i;
#ifndef NOUNIXDOM
	struct sockaddr_un uns;
#endif
	char buf[256];

	pvm_bufinfo(mid, (int *)0, (int *)0, &src);

	pvm_upkint(&ttpro, 1, 1);
	pvm_upkint(&ackd, 1, 1);
	pvm_upkstr(buf);

	if (pcbp = ttpcb_find(src)) {
		if (pcbp->tt_state == TTCONWAIT) {
			if (pvmdebmask & PDMROUTE) {
				pvmlogprintf(
						"pvm_tc_conack() CONACK from t%x ackd=%d\n",
						src, ackd );
			}
			if (ttpro != TDPROTOCOL) {
				pvmlogprintf("pvm_tc_conack() t-t protocol mismatch with t%x\n",
						pcbp->tt_tid);
				ackd = 1;

			} else {
				if (ackd != 0) {
					if (pvmdebmask & PDMROUTE) {
						pvmlogprintf("pvm_tc_conack() route to t%x denied\n",
								pcbp->tt_tid);
					}

				} else {
					if (buf[0] == '/') {
#ifdef NOUNIXDOM
						pvmlogprintf(
						"pvm_tc_conack() CONREQ from t%x, Unix domain socket unsupported\n",
								src);
						ackd = 1;
#else /*NOUNIXDOM*/
						BZERO((char*)&uns, sizeof(uns));
						uns.sun_family = AF_UNIX;
						strcpy(uns.sun_path, buf);
						while ((i = connect(pcbp->tt_fd, (struct sockaddr*)&uns,
								sizeof(uns))) == -1
								&& errno == EINTR)
							;
						if (i == -1) {
							pvmlogperror("pvm_tc_conack() connect");
							ackd = 1;

						} else {
							pcbp->tt_state = TTOPEN;
#ifndef WIN32	
							if ((i = fcntl(pcbp->tt_fd, F_GETFL, 0)) == -1)
								pvmlogperror("pvm_tc_conack() fcntl");
							else {
#ifdef O_NDELAY
								i |= O_NDELAY;
#else
								i |= FNDELAY;
#endif
								(void)fcntl(pcbp->tt_fd, F_SETFL, i);
							}
#endif
							pvm_fd_add(pcbp->tt_fd, 1);
						}

#endif /*NOUNIXDOM*/

					} else {
						pcbp->tt_osad.sin_family = AF_INET;
						hex_inadport(buf, &pcbp->tt_osad);
						while ((i = connect(pcbp->tt_fd,
								(struct sockaddr*)&pcbp->tt_osad,
								sizeof(pcbp->tt_osad))) == -1
								&& errno == EINTR)
							;
						if (i == -1) {
							pvmlogperror("pvm_tc_conack() connect");
							ackd = 1;

						} else {
							pcbp->tt_state = TTOPEN;
#ifndef NOSOCKOPT
							if (setsockopt(pcbp->tt_fd, SOL_SOCKET, SO_LINGER,
									(char*)linger, sizeof(linger)) == -1)
								pvmlogperror("pvm_tc_conack() setsockopt");
#endif /*NOSOCKOPT*/

#ifndef WIN32
							if ((i = fcntl(pcbp->tt_fd, F_GETFL, 0)) == -1)
								pvmlogperror("pvm_tc_conack() fcntl");
							else {
#ifdef O_NDELAY
								i |= O_NDELAY;
#else
								i |= FNDELAY;
#endif
								(void)fcntl(pcbp->tt_fd, F_SETFL, i);
							}
#endif
							pvm_fd_add(pcbp->tt_fd, 1);
						}
					}
				}
			}

			if (ackd != 0) {
				pcbp->tt_state = TTDENY;
				(void)close(pcbp->tt_fd);
				pcbp->tt_fd = -1;
			}
			else if (pvmdebmask & PDMROUTE) {
				pvmlogprintf( "%s: connection accepted to t%x\n",
					"pvm_tc_conack()", src );
			}

		} else {
			pvmlogprintf("pvm_tc_conack() CONACK from t%x but state=%d\n",
					src, pcbp->tt_state);
		}

	} else {
		pvmlogprintf("pvm_tc_conack() suprious CONACK from t%x\n", src);
	}

	pvm_freebuf(mid);
	return 0;
}
