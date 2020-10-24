void
pvmbailout(n)
	int n;
{
	struct task *tp;

	pvmlogprintf("pvmbailout(%d)\n", n);

	/* sockaddr file */

	if (loclsnam)
		(void)unlink(loclsnam);

	/* kill local tasks */

#ifdef SHMEM
	mpp_cleanup();
#endif

	if (locltasks)
		for (tp = locltasks->t_link; tp != locltasks; tp = tp->t_link) {
			if (tp->t_pid)
#ifndef WIN32
#ifdef IMA_OS2
				(void)os2_kill(tp->t_pid, SIGTERM);
#else
				(void)kill(tp->t_pid, SIGTERM);
#endif
#else
				(void)kill(tp->t_pid,tp->t_handle,SIGTERM);
#endif
			if (tp->t_authnam)
				(void)unlink(tp->t_authnam);
		}

	/* shutdown slave pvmds / notify master */

	if (netsock != -1) {
		char dummy[DDFRAGHDR];
		int hh;
		struct hostd *hp;

		if (pvmdebmask)
			pvmlogerror("sending FIN|ACK to all pvmds\n");
		for (hh = hosts->ht_last; hh >= 1; hh--)
			if ((hp = hosts->ht_hosts[hh]) && hp->hd_hostpart != myhostpart) {
				pvmput32(dummy, hp->hd_hostpart | TIDPVMD);
				pvmput32(dummy + 4, myhostpart | TIDPVMD);
				pvmput16(dummy + 8, 0);
				pvmput16(dummy + 10, 0);
				pvmput8(dummy + 12, FFFIN|FFACK);
				sendto(netsock, dummy, DDFRAGHDR, 0,
						(struct sockaddr*)&hp->hd_sad, sizeof(hp->hd_sad));
			}
	}

#ifndef NOUNIXDOM
	if (loclspath)
		(void)unlink(loclspath);
#endif

	if (n < 0)
		abort();
	exit(n);
}
