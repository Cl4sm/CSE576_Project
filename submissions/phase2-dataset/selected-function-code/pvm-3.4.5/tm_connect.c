int
tm_connect(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int ver;						/* task's libpvm t-d proto version */
	char authfn[PVMTMPNAMLEN];	/* t-auth file name */
	int d;
	int cc;

	if (upkint(mp, &ver) || upkstr(mp, authfn, sizeof(authfn))) {
		pvmlogerror("tm_connect() bad msg format\n");
		goto bail;
	}

	/*
	*	if protocols are not compatible, send nack
	*	context will get flushed after reply is sent
	*/

	if (ver != TDPROTOCOL) {
		pvmlogprintf("tm_connect() t-d protocol mismatch (%d/%d)\n",
			ver, TDPROTOCOL);

		mp = replymessage(mp);
		pkint(mp, TDPROTOCOL);
		pkint(mp, 0);
		pkstr(mp, "");
		mp->m_flag |= MM_PRIO;
		mesg_to_task(tp, mp);
		pmsg_unref(mp);
		goto bail;
	}

#ifdef NOPROT
	tp->t_authnam = TALLOC(PVMTMPNAMLEN, char, "auth");
	(void)PVMTMPNAMFUN(tp->t_authnam);
#else

	/*
	*	write in t-auth file, create empty d-auth file that task
	*	must write and we'll check later
	*/

	if ((d = open(authfn, O_WRONLY, 0)) == -1) {
		pvmlogperror("tm_connect() can't open t-auth file");
		goto bail;
	}
	cc = write(d, authfn, 1);
	if (cc != 1) {
		if (cc == -1)
			pvmlogperror(authfn);
		pvmlogerror("tm_connect() can't write t-auth file\n");
	}
	(void)close(d);

	tp->t_authnam = TALLOC(PVMTMPNAMLEN, char, "auth");
	(void)PVMTMPNAMFUN(tp->t_authnam);

#ifndef IMA_OS2
	if ((tp->t_authfd = open(tp->t_authnam, O_RDONLY|O_CREAT|O_TRUNC, 0600))
#else
	if ((tp->t_authfd = open(tp->t_authnam, O_RDWR|O_CREAT|O_TRUNC, 0600))
#endif
	== -1) {
		pvmlogperror("tm_connect() can't create d-auth file");
		PVM_FREE(tp->t_authnam);
		tp->t_authnam = 0;
		goto bail;
	}

#endif

	/*
	*	task's turn to auth
	*/

	tp->t_flag |= TF_AUTH;

	mp = replymessage(mp);
	pkint(mp, TDPROTOCOL);
	pkint(mp, 1);
	pkstr(mp, tp->t_authnam);
	mp->m_flag |= MM_PRIO;
	mesg_to_task(tp, mp);
	pmsg_unref(mp);
	return 0;

bail:
	tp->t_flag |= TF_CLOSE;
	return 0;
}
