static int
shmemuptod(txup)
	struct pmsg *txup;			/* outgoing message */
{
	struct frag *txfp = 0;		/* cur ogm frag or null */
	struct timeval tin;
	struct timeval tnow;
	struct timeval *tvp;
#ifdef FDSETNOTSTRUCT
	fd_set rfds, wfds;
#else
	struct fd_set rfds, wfds;
#endif
	int ff;
	int n;

	char *txcp = 0;				/* point to remainder of txfp */
	int txtogo = 0;				/* len of remainder of txfp */
	struct sockaddr_in sad;
	int s;
	struct msgid *sendmsg = (struct msgid *) NULL;
	char errtxt[64];
	int upfd;					/* one-way socket to the daemon */

	if (txup) {
	/* transmitting a umsg */
		txfp = txup->m_frag->fr_link;
		if (!txfp->fr_buf) {
			if (!(txfp = fr_new(MAXHDR)))
				return PvmNoMem;
			txfp->fr_dat += MAXHDR;
			LISTPUTBEFORE(txup->m_frag, txfp, fr_link, fr_rlink);
		}

		/* To the daemon ? */
		upfd = pvmdsock;

		/* txup->m_ref++; */
	}

	if (pvmdebmask & PDMMESSAGE) {
		pvmlogprintf("shmemuptod() mid %d", (txup ? txup->m_mid : 0));
		if (txup) {
			pvmlogprintf(" ctx %d tag %s",
					txup->m_ctx,
					pvmnametag(txup->m_tag, (int *)0));
		}
	}

	/* must block sending to pvmd */
	tvp = (struct timeval *) NULL;

	/* We haven't sent anything, yet, Build Header */

	txcp = txfp->fr_dat; /* packed data */
	/* XXXX we assume thats its only packed data TMs */

	txtogo = txfp->fr_len;	/* actual data size */

	/*
	 * This is first and last frag. So prepend message header
	 */
	txcp -= MSGHDRLEN;
	txtogo += MSGHDRLEN;
	pvmput32(txcp,
			(txup->m_enc == 0x20000000 ? pvmmydsig : txup->m_enc));
	pvmput32(txcp + 4, txup->m_tag);
	pvmput32(txcp + 8, txup->m_ctx);
	pvmput32(txcp + 16, txup->m_wid);
#ifdef	MCHECKSUM
	pvmput32(txcp + 20, umbuf_crc(txup));
#else
	pvmput32(txcp + 20, 0);
#endif
	ff = FFSOM | FFEOM;	/* Alpha Omega: first and last frag man */

	/*
	 * prepend frag header
	 */
	txcp -= TDFRAGHDR;
	pvmput32(txcp, txup->m_dst);
	pvmput32(txcp + 4, pvmmytid);
	pvmput32(txcp + 8, txtogo);
	pvmput32(txcp + 12, 0);		/* to keep putrify happy */
	pvmput8(txcp + 12, ff);
	txtogo += TDFRAGHDR;

	/*
	 * Make sure we have a valid socket address 
	 * Prod will connect us to the daemon if we are not already
	 * connected.
	 */

	if ( pvmdsock < 0 ) {

		prodpvmd();			/* connect to the daemon setting pvmdsock */

		if ( pvmdsock < 0 ) {	/* nested yuck.. */
			pvmlogerror("shmemuptod() Can't get daemon socket open\n");
		    pmsg_unref(txup);   /* unref the message */
			return -1;
		}
	}

	if (pvmdebmask & PDMPACKET) {
		pvmlogprintf("shmemuptod() dst t%x n=%d\n",
				txup->m_dst, txtogo);
	}

	/* XXX yep... blocking loop forever!!!!! */
	/* If I can't get 100+ bytes up that socket we are */
	/* in trouble anyway! */
	while (txtogo) {

#if defined(IMA_RS6K) || defined(IMA_SP2MPI) || defined(IMA_AIX4) \
		|| defined (IMA_AIX4MP) || defined(IMA_AIX5MP) \
		|| defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
		n = write(pvmdsock, txcp, min(txtogo, 4096));
#else
		n = write(pvmdsock, txcp, txtogo);
#endif

		if (pvmdebmask & PDMPACKET)
			pvmlogprintf("shmemuptod() wrote %d\n", n);

		if (n == -1 && errno != EWOULDBLOCK && errno != EINTR)
		{
			pvmlogperror("shmemuptod() write pvmd sock");
			return PvmSysErr;
		}

		if (n > 0)
		{
			txcp += n;
			txtogo -= n;
		}

	} /* while we have data to send */


	/* we made it here so the message went! */

	pmsg_unref(txup);	/* unref the message */

	return 0;
}
