	int num;
	struct hst **hostlist;
{
	int nxth = 0;						/* next host in list to start */
	struct slot *slact = 0;				/* active list of slots */
	struct hst *hp;
	struct slot *sp, *sp2;
	struct timeval tnow;
	struct timeval tout;
#ifdef FDSETNOTSTRUCT
	fd_set rfds;						/* emx headers */
#else
	struct fd_set rfds;
#endif
	int nfds;
	int i;
	int n;
	char *p;

	/* init slot free list */

	slfree = &slots[RSHNPLL+1];
	slfree->s_link = slfree->s_rlink = slfree;
	slact = &slots[RSHNPLL];
	slact->s_link = slact->s_rlink = slact;
	for (i = RSHNPLL; i-- > 0; ) {
		LISTPUTAFTER(slfree, &slots[i], s_link, s_rlink);
	}

	/*
	* keep at this until all hosts in table are completed
	*/

	for (; ; ) {

		/*
		* if empty slots, start on new hosts
		*/

		for (; ; ) {

			/* find a host for slot */

			if (slfree->s_link != slfree && nxth < num)
				hp = hostlist[nxth++];
			else
				break;

			sp = slfree->s_link;
			LISTDELETE(sp, s_link, s_rlink);
			sp->s_hst = hp;
			sp->s_len = 0;
			sp->s_elen = 0;
			if (pvmdebmask & PDMSTARTUP) {
				pvmlogprintf("pl_startup() trying %s\n", hp->h_name);
			}
			phase1(sp);
			if (hp->h_result) {
				/* error or fully started (manual startup) */

				if (pvmdebmask & PDMSTARTUP) {
					pvmlogprintf(
					"pl_startup() got result \"%s\" for %s phase1()\n",
						hp->h_result, hp->h_name);
				}

				LISTPUTBEFORE(slfree, sp, s_link, s_rlink);

			} else {
				/* partially started */

				LISTPUTBEFORE(slact, sp, s_link, s_rlink);
				pvmgetclock(&sp->s_bail);
				tout.tv_sec = RSHTIMEOUT;
				tout.tv_usec = 0;
				TVXADDY(&sp->s_bail, &sp->s_bail, &tout);
			}
		}

		/* if no hosts in progress, we are finished */

		if (slact->s_link == slact)
			break;

		/*
		* until next timeout, get output from any slot
		*/

		FD_ZERO(&rfds);
		nfds = 0;
		TVCLEAR(&tout);
		pvmgetclock(&tnow);
		for (sp = slact->s_link; sp != slact; sp = sp->s_link) {
			if (TVXLTY(&sp->s_bail, &tnow)) {
				pvmlogprintf("pl_startup() giving up on host %s after %d secs\n",
						sp->s_hst->h_name, RSHTIMEOUT);
				sp->s_hst->h_result = STRALLOC("PvmCantStart");
				sp2 = sp->s_rlink;
				close_slot(sp);
				sp = sp2;
				continue;
			}

			if (!TVISSET(&tout) || TVXLTY(&sp->s_bail, &tout))
				tout = sp->s_bail;
			if (sp->s_rfd >= 0)
				FD_SET(sp->s_rfd, &rfds);
			if (sp->s_rfd > nfds)
				nfds = sp->s_rfd;
			if (sp->s_efd >= 0)
				FD_SET(sp->s_efd, &rfds);
			if (sp->s_efd > nfds)
				nfds = sp->s_efd;
		}

		if (slact->s_link == slact)
			break;

		nfds++;

		if (TVXLTY(&tnow, &tout)) {
			TVXSUBY(&tout, &tout, &tnow);
		} else {
			TVCLEAR(&tout);
		}
		if (pvmdebmask & PDMSTARTUP) {
			pvmlogprintf("pl_startup() select timeout is %d.%06d\n",
					tout.tv_sec, tout.tv_usec);
		}
		if ((n = select(nfds,
#ifdef	FDSETISINT
				(int *)&rfds, (int *)0, (int *)0,
#else
				(fd_set *)&rfds, (fd_set *)0, (fd_set *)0,
#endif
				&tout)) == -1) {
			if (errno != EINTR) {
				pvmlogperror("pl_startup() select");
				pvmbailout(0);
			}
		}
		if (pvmdebmask & PDMSTARTUP) {
			pvmlogprintf("pl_startup() select returns %d\n", n);
		}
		if (n < 1) {
			if (n == -1 && errno != EINTR) {
				pvmlogperror("pl_startup() select");
				pvmbailout(0);	/* XXX this is too harsh */
			}
			continue;
		}

		/*
		* check for response on stdout or stderr of any slave.
		*/

		for (sp = slact->s_link; sp != slact; sp = sp->s_link) {

			/*
			* stderr ready.  log output with remote's host name.
			*/
			if (sp->s_efd >= 0 && FD_ISSET(sp->s_efd, &rfds)) {
				n = read(sp->s_efd, sp->s_ebuf + sp->s_elen,
						sizeof(sp->s_ebuf) - sp->s_elen - 1);
				if (n > 0) {
					sp->s_elen += n;
					sp->s_ebuf[sp->s_elen] = 0;
					while (p = CINDEX(sp->s_ebuf, '\n')) {
						*p = 0;
						pvmlogprintf("stderr@%s: %s\n",
								sp->s_hst->h_name, sp->s_ebuf);
						p++;
						sp->s_elen -= p - sp->s_ebuf;
						if (sp->s_elen > 0) {
							BCOPY(p, sp->s_ebuf, sp->s_elen);
							sp->s_ebuf[sp->s_elen] = 0;
						}
					}
					if (sp->s_elen == sizeof(sp->s_ebuf) - 1) {
						pvmlogprintf("stderr@%s: %s\n",
								sp->s_hst->h_name, sp->s_ebuf);
						sp->s_elen = 0;
					}

				} else {
					if (sp->s_elen > 0) {
						pvmlogprintf("stderr@%s: %s\n",
								sp->s_hst->h_name, sp->s_ebuf);
						sp->s_elen = 0;
					}
					(void)close(sp->s_efd);
					sp->s_efd = -1;
				}
			}

			/*
			* stdout ready.
			* look for a complete line starting with "ddpro".
			*/
			if (sp->s_rfd >= 0 && FD_ISSET(sp->s_rfd, &rfds)) {
				n = read(sp->s_rfd, sp->s_buf + sp->s_len,
						sizeof(sp->s_buf) - sp->s_len - 1);
				if (n > 0) {
					sp->s_len += n;
					sp->s_buf[sp->s_len] = 0;
					while (p = CINDEX(sp->s_buf, '\n')) {
						*p = 0;
						if (!strncmp(sp->s_buf, "ddpro", 5)) {
							if (pvmdebmask & PDMSTARTUP) {
								pvmlogprintf("stdout@%s: %s\n",
										sp->s_hst->h_name, sp->s_buf);
							}
							sp->s_hst->h_result = STRALLOC(sp->s_buf);
							break;

						} else {
							pvmlogprintf("stdout@%s: %s\n",
									sp->s_hst->h_name, sp->s_buf);
							p++;
							sp->s_len -= p - sp->s_buf;
							if (sp->s_len > 0) {
								BCOPY(p, sp->s_buf, sp->s_len);
								sp->s_buf[sp->s_len] = 0;
							}
						}
					}
					if (sp->s_len == sizeof(sp->s_buf) - 1) {
						pvmlogprintf("stdout@%s: %s\n",
								sp->s_hst->h_name, sp->s_buf);
						sp->s_len = 0;
					}

				} else {
					if (sp->s_len > 0) {
						pvmlogprintf("stdout@%s: %s\n",
								sp->s_hst->h_name, sp->s_buf);
						sp->s_len = 0;
					}
					if (n) {
						pvmlogprintf("stdout@%s",
								sp->s_hst->h_name);
					} else {
						pvmlogprintf("stdout@%s: EOF\n",
								sp->s_hst->h_name);
					}
					if (sp->s_elen > 0) {
						pvmlogprintf("stderr@%s: %s\n",
								sp->s_hst->h_name, sp->s_ebuf);
						sp->s_elen = 0;
					}

					/* before failing, check for alternate WIN32 cmd */
					if (sp->s_hst->h_wincmd) {
						if (pvmdebmask & PDMSTARTUP) {
							pvmlogprintf(
								"pl_startup() re-trying %s as WIN32\n",
								hp->h_name);
						}
						/* no need to free h_cmd, we're a pvmd'... */
						sp->s_hst->h_cmd = sp->s_hst->h_wincmd;
						sp->s_hst->h_wincmd = 0;

						phase1(sp);

						if (!(sp->s_hst->h_result)) {
							/* partially started */
							pvmgetclock(&sp->s_bail);
							tout.tv_sec = RSHTIMEOUT;
							tout.tv_usec = 0;
							TVXADDY(&sp->s_bail, &sp->s_bail, &tout);
						}

					} else {
						sp->s_hst->h_result = STRALLOC("PvmCantStart");
					}
				}
				if (sp->s_hst->h_result) {
					sp2 = sp->s_rlink;
					close_slot(sp);
					sp = sp2;
					continue;
				}
			}
		}
	}
	return 0;
}
