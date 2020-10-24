loclconn()
{
	struct task *tp;			/* new task context */
#ifdef SOCKLENISUINT
#if defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2) \
		|| defined(IMA_AIX56K64) || defined(IMA_LINUXALPHA)
	unsigned int oslen;
#else
	size_t oslen;
#endif
#else
	int oslen;
#endif
	int i;
#ifndef NOUNIXDOM
	struct sockaddr_un uns;
#endif

/*
#ifdef SHMEM
 	return 0;
	just getma outa here boy!
#endif
*/

	if ((tp = task_new(0)) == NULL) {
		pvmlogprintf("loclconn() too many tasks?\n" );
		return PvmOutOfRes;
	}

#ifdef NOUNIXDOM
	tp->t_salen = sizeof(tp->t_sad);

	oslen = sizeof(tp->t_sad);
	if ((tp->t_sock = accept(loclsock, (struct sockaddr*)&tp->t_sad,
			&oslen)) == -1) {
		pvmlogperror("loclconn() accept");
		task_free(tp);
		tp = 0;

	} else {
		if (pvmdebmask & (PDMPACKET|PDMTASK)) {
			pvmlogprintf("loclconn() accept from %s sock %d\n",
					inadport_decimal(&tp->t_sad), tp->t_sock);
		}
#ifndef NOSOCKOPT
		i = 1;
		if (setsockopt(tp->t_sock, IPPROTO_TCP, TCP_NODELAY,
				(char*)&i, sizeof(int)) == -1) {
			pvmlogperror("loclconn() setsockopt");
		}
#endif
	}

#else /*NOUNIXDOM*/
	oslen = sizeof(uns);
	if ((tp->t_sock = accept(loclsock, (struct sockaddr*)&uns, &oslen)) == -1) {
		pvmlogperror("loclconn() accept");
		task_free(tp);
		tp = 0;

	} else {
		if (pvmdebmask & (PDMPACKET|PDMTASK))
			pvmlogerror("loclconn() accept\n");
	}

#endif /*NOUNIXDOM*/

	if (tp) {
#ifndef WIN32
		if ((i = fcntl(tp->t_sock, F_GETFL, 0)) == -1)
			pvmlogperror("loclconn: fcntl");
		else {
#ifdef	IMA_RS6K
	/* did you ever feel as though your mind had started to erode? */
			i |= O_NONBLOCK;
#else	/*IMA_RS6K*/
#ifdef O_NDELAY
			i |= O_NDELAY;
#else
			i |= FNDELAY;
#endif
#endif	/*IMA_RS6K*/
			(void)fcntl(tp->t_sock, F_SETFL, i);
		}
#endif
		wrk_fds_add(tp->t_sock, 1);
	}

	return 0;
}
