static int
mxfer(txup, tmout)
	struct pmsg *txup;			/* outgoing message or null */
	struct timeval *tmout;		/* time limit to get at least one message */
{
	struct ttpcb *txpcbp = 0;	/* route for ogm */
	struct frag *txfp = 0;		/* cur ogm frag or null */
	struct timeval tin;
	struct timeval tnow;
	struct timeval *tvp;
#ifdef FDSETNOTSTRUCT
	fd_set rfds, wfds;
#else
	struct fd_set rfds, wfds;
#endif
	int wantmore = 1;			/* nothing rxd yet and not timed out */
	int gotem = 0;				/* count of received msgs downloaded */
	int ff;
	int n;
#ifdef SOCKLENISUINT
#if defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	unsigned int oslen;
#else
	size_t oslen;
#endif
#else
	int oslen;
#endif
	char *txcp = 0;				/* point to remainder of txfp */
	int txtogo = 0;				/* len of remainder of txfp */
	struct sockaddr_in sad;
	int s;
	struct ttpcb *pcbp;
	char *inPlaceHeader = (char  *) NULL;	/* for inplace data */
	int inPlaceBodyLen = 0;					/* len of inplace body */	
	int bypassRead = FALSE;				/* bypass reading packets before send?*/
	int waitForOgmToComplete = FALSE;
	int probedForIncomingPkts = FALSE;	/* need to probe for incoming pkts */ 
	struct msgid *sendmsg = (struct msgid *) NULL;
	char errtxt[64];

	int nfr = 0, totfr = 0;				/* number frags read my mxinput */
	int probe = 0;						/* called at a probe (nrecv) */

	if (tmout)
	{
		pvmgetclock(&tin);
		probe = ( tmout -> tv_sec == 0 && tmout -> tv_usec == 0);
	}

	if (txup) {	/* transmitting a umsg */
		txfp = txup->m_frag->fr_link;
		if (!txfp->fr_buf) {
			if (!(txfp = fr_new(MAXHDR)))
				return PvmNoMem;
			txfp->fr_dat += MAXHDR;
			LISTPUTBEFORE(txup->m_frag, txfp, fr_link, fr_rlink);
		}
		/* Get connection info for this dest. If none, go to pvmd */
		if (!(txpcbp = ttpcb_find(txup->m_dst)) || txpcbp->tt_state != TTOPEN)
			txpcbp = topvmd;
		txup->m_ref++;
		bypassRead = TRUE;		/* bypass reading the first time through */
	}

	if (pvmdebmask & PDMMESSAGE) {
		pvmlogprintf("mxfer() mid %d", (txup ? txup->m_mid : 0));
		if (txup)
			pvmlogprintf(" ctx %d tag %s dst t%x route t%x",
					txup->m_ctx,
					pvmnametag(txup->m_tag, (int *)0),
					txup->m_dst, txpcbp->tt_tid);
		if (tmout)
			pvmlogprintf(" tmout %d.%06d\n",
					tmout->tv_sec, tmout->tv_usec);
		else
			pvmlogprintf(" tmout inf\n");
	}

/* XXX we go an extra time through select because wantmore gets set
   XXX inside loop.  no big deal.  also, could avoid calling gettimeofday
   XXX if tmout is 0.0 */

	/* Do this loop if
	 *		1) Sending a frag
	 * OR	2) Received *part* of an incoming frag from the daemon
	 * OR	3) Wantmore is true
	 * OR 	4) Need the current OGM to complete its asynch send 
	 * OR   5) We haven't probed at least once for incoming packets
	 *
	 * wantmore is initialized to  TRUE  -> loop exec'ed at least once 
	*/


	while (txfp || topvmd->tt_rxf || wantmore || waitForOgmToComplete
			|| !probedForIncomingPkts
	) 
	{

		totfr = 0; 

		/* Either we are sending something, or have a partial recv'd frag */
		if (txfp || topvmd->tt_rxf) 			/* gotta block */
		{
	 	 	/* must block if sending any message, or receiving from pvmd */
			tvp = (struct timeval *) NULL;

		} 
		else 
		{
			if (wantmore) 
			{
				/* Calculate how long to wait inside the "select"
				 *	tval = NULL means wait forever
				 *  tval = (0,0) means probe
				 * 	tval = (x,y) means wait at most x sec + y usec
				*/

				if (tmout) 					/* finite time to block */
				{
					pvmgetclock(&tnow);
					TVXSUBY(&tnow, &tnow, &tin);
					if (TVXLTY(tmout, &tnow)) 	/* time's up */
					{
						TVCLEAR(&tnow);
						wantmore = 0;
					} 
					else 
					{
						TVXSUBY(&tnow, tmout, &tnow);
					}
					tvp = &tnow;
				} 
				else 
				{						/* forever to block */
					tvp = (struct timeval *) NULL;
				}

			} 
			else 
			{							/* return asap */
				TVCLEAR(&tnow);
				tvp = &tnow;
			}
		}

		/* Here we are setting the read and write file descriptors for
		 *	the following select statement. 
		*/


#if !defined(IMA_MPP)
		rfds = pvmrfds;
		FD_ZERO(&wfds);
		if (txfp)
			FD_SET(txpcbp->tt_fd, &wfds);
#endif

		if (pvmdebmask & PDMSELECT) {
			if (tvp)
				pvmlogprintf("mxfer() select timeout %d.%06d\n",
						tvp->tv_sec, tvp->tv_usec);
			else
				pvmlogprintf("mxfer() select timeout inf\n");
#if !defined(IMA_MPP)
			print_fdset("mxfer() rfds=", pvmnfds, &rfds);
			print_fdset("mxfer() wfds=", pvmnfds, &wfds);
#endif
		}

#if !defined(IMA_MPP)
		probedForIncomingPkts = TRUE;
		if ((n = select(pvmnfds,
#ifdef	FDSETISINT
				(int *)&rfds, (int *)&wfds, (int *)0,
#else
				(fd_set *)&rfds, (fd_set *)&wfds, (fd_set*)0,
#endif
				tvp)) == -1
		&& errno != EINTR
#ifdef IMA_LINUX
		&& errno != EAGAIN
#endif
		) {
			pvmlogperror("mxfer() select");
			return PvmSysErr;
		}
		if (pvmdebmask & PDMSELECT) {
			pvmlogprintf("mxfer() select returns %d\n", n);
		}
		if (n == -1)
			continue;
	/*
	* if pvmd conn has data ready, read packets
	*/

		if (FD_ISSET(topvmd->tt_fd, &rfds) 
			&& !(mxfersingle && gotem)) 
		{
			if ((n = mxinput(topvmd, &nfr)) < 0) {
				if (n != -1)
					pvmlogerror("mxfer() mxinput bad return on pvmd sock\n");
				else if (pvmdebmask & (PDMSELECT|PDMMESSAGE|PDMPACKET))
					pvmlogerror("mxfer() EOF on pvmd sock\n");
				return PvmSysErr;
			}

			totfr += nfr;
			if (gotem += n)
				wantmore = 0;
		}

	/*
	* if task conn has data ready, read packets
	*/

	/* This goes through *all* of the pcbs that have been defined to
	 *	check for messages.  This is too much overhead for IMA_MPP
	 *	machines. Instead all local routes must be probed at one time.
	*/ 
		
		for (pcbp = ttlist->tt_link; pcbp != ttlist; pcbp = pcbp->tt_link) {
			if (pcbp->tt_state == TTOPEN && FD_ISSET(pcbp->tt_fd, &rfds)) {
				if ((n = mxinput(pcbp, &nfr)) < 0)
					ttpcb_dead(pcbp);

				else {
					totfr += nfr;
					if (gotem += n)
						wantmore = 0;
				}

			} else /* Try and complete a direct (socket) Connection rqst */ 
				if (pcbp->tt_state == TTGRNWAIT
				&& FD_ISSET(pcbp->tt_fd, &rfds)) {
					oslen = sizeof(sad);
					s = accept(pcbp->tt_fd, (struct sockaddr*)&sad, &oslen);
					if (s == -1) {
						pvmlogperror("mxfer() accept");
						ttpcb_dead(pcbp);

					} else {
#ifndef NOSOCKOPT
#ifndef NOUNIXDOM
						if (!pcbp->tt_spath) {	/* not unix sock */
#endif
							n = 1;
							if (setsockopt(s, IPPROTO_TCP, TCP_NODELAY,
									(char*)&n, sizeof(int)) == -1)
								pvmlogperror("mxfer() setsockopt");
#if (!defined(IMA_RS6K)) \
		&& (!defined(IMA_AIX46K)) && (!defined(IMA_AIX46K64)) \
		&& (!defined(IMA_AIX56K)) && (!defined(IMA_AIX56K64))
							n = TTSOCKBUF;
							if (setsockopt(s, SOL_SOCKET, SO_SNDBUF,
									(char*)&n, sizeof(int)) == -1
							|| setsockopt(s, SOL_SOCKET, SO_RCVBUF,
									(char*)&n, sizeof(n)) == -1)
								pvmlogperror(
									"mxfer() setsockopt SO_SNDBUF");
#endif
#ifndef NOUNIXDOM
						}
#endif
#endif /*NOSOCKOPT*/
						if (pvmdebmask & PDMROUTE) {
							pvmlogprintf("mxfer() accept from t%x = %d\n",
									pcbp->tt_tid, s);
						}
						(void)close(pcbp->tt_fd);
/*
						(void)dup2(s, pcbp->tt_fd);
						(void)close(s);
*/
						pvm_fd_delete(pcbp->tt_fd, 1);

	/* notify route socket closed */

						check_routedelete(pcbp);
						pcbp->tt_fd = s;
						pcbp->tt_state = TTOPEN;

#ifndef WIN32
						{
							int i;

							if ((i = fcntl(pcbp->tt_fd, F_GETFL, 0))
									== -1)
								pvmlogperror("mxfer() fcntl");
							else {
#ifdef O_NDELAY
								i |= O_NDELAY;
#else
								i |= FNDELAY;
#endif
								(void)fcntl(pcbp->tt_fd, F_SETFL, i);
							}
						}
#endif

						pvm_fd_add(s, 1);

	/* new route socket created for communication */

						check_routeadd(pcbp);
					}
				}
		}

		bypassRead = bypassRead; /* sgi compiler */

#else	/* !IMA_MPP */

		/* For Native Messaging mxinput is used to probe pvmd AND tasks when 
		 * given a NULL ttpcb pointer 
		*/

		if (!bypassRead)
		{
	
			probedForIncomingPkts = TRUE;
			if ((n = mxinput((struct ttpcb *) NULL, &nfr)) < 0) {
					pvmlogerror("mxfer() mxinput bad return on probe\n");
					return PvmSysErr;
				}

			totfr += nfr;
			if (gotem += n)
				wantmore = 0;
		}
		

#endif /* !IMA_MPP */
	/*
	* if sending and socket ready to write, send packets
	*/

#if !defined(IMA_MPP)
		/* XXX Native message direct routes are alway open, if they exist */

		if (txfp && txpcbp->tt_state != TTOPEN) 
		{
		/* make sure our route hasn't just been closed */
			txpcbp = topvmd;
			txcp = 0;
			txtogo = 0;
			txfp = txup->m_frag->fr_link;

		} 
		else 
#endif
		{
			if (txfp 
				&& !waitForOgmToComplete
#if !defined(IMA_MPP)
				&& FD_ISSET(txpcbp->tt_fd, &wfds) 
#endif
			){
				inPlaceHeader = (char *) NULL; 		/*sending inplace header ?*/
				inPlaceBodyLen = 0;	   /* length of inplace body, if inplace*/

				if (!txtogo)  /* We haven't sent anything, yet, Build Header */
				{
					if (txfp->fr_u.dab)			/* packed data */
					{
						txcp = txfp->fr_dat;
					}
					else						/* inplace data */
					{
						txcp = inPlaceHeader = inPlaceHdrPool + 
								hdrPoolStk*(HEADER_SIZE+SHORT_PAYLOAD) + HEADER_SIZE; 
									
						if (pvmdebmask & PDMPACKET)
						{
							sprintf(errtxt,"mxfer() hdrPoolStk is %d\n", hdrPoolStk);
							pvmlogerror(errtxt);
						}

						if (hdrPoolStk++ >= NINPLACE_HDRS)
						{
							sprintf(errtxt, "mxfer(): > %d inplace frags \n",
									NINPLACE_HDRS);
							pvmlogerror(errtxt);
							return PvmSysErr;
						}

					}

					txtogo = txfp->fr_len;

					/*
				 	 * if this is first frag, prepend message header
					*/
					ff = 0;
					if (txfp->fr_rlink == txup->m_frag) 
					{
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
						ff = FFSOM;
					}
					if (txfp->fr_link == txup->m_frag)
						ff |= FFEOM;
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
					if (!txfp->fr_u.dab)			/* inplace data */
					{
						txtogo -= txfp->fr_len;
						inPlaceBodyLen = txfp->fr_len;
					}
				}

				if (pvmdebmask & PDMPACKET) 
				{
					pvmlogprintf("mxfer() dst t%x n=%d\n", txup->m_dst, txtogo);
				}
				
#if defined(IMA_MPP)

				/* node_send will do an async send. Feed it a midlist struct. 
				 * when we get to the end of a message we call ogm_done
				 * to make sure message is really gone, and we can free	
				 * memory 
				 *	
				*/
				n = pvm_node_send(txcp, txtogo, txpcbp, &sendmsg, 
							inPlaceHeader, inPlaceBodyLen);
#else 
				inPlaceBodyLen = inPlaceBodyLen; /* sgi compiler */

#if defined(IMA_RS6K) || defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
		|| defined(IMA_AIX5SP2)
				n = write(txpcbp->tt_fd, txcp, min(txtogo, 4096));
#else
#ifndef WIN32
				n = write(txpcbp->tt_fd, txcp, txtogo);
#else 
				n = win32_write_socket(txpcbp->tt_fd,txcp,txtogo);
#endif
#endif

#endif
				if (pvmdebmask & PDMPACKET) {
					pvmlogprintf("mxfer() wrote %d\n", n);
				}
				if (n == -1 && 
#ifndef WIN32
						errno != EWOULDBLOCK && 
#endif
						errno != EINTR) 
				{
					if (txpcbp == topvmd) 
					{
						pvmlogperror("mxfer() write pvmd sock");
						return PvmSysErr;
					} 
					else 
					{
						pvmlogperror("mxfer() write tt sock");
						/* reset message and route to pvmd */
						ttpcb_dead(txpcbp);
						txpcbp = topvmd;
						txcp = 0;
						txtogo = 0;
						txfp = txup->m_frag->fr_link;
					}
				}

				if (n > 0) 
				{
					txcp += n;
					if ((txtogo -= n) <= 0) 
					{
						if (txcp == inPlaceHeader && txfp->fr_len ) 	
						/* inplace, body length > 0  */
						{
							txcp = txfp->fr_dat;
							txtogo = txfp->fr_len;
							bypassRead = TRUE; /* try to send body without 
													doing another read */ 
						}
						else						/* packed */ 
						{
							txcp = 0;
							txfp = txfp->fr_link;
							if (!txfp->fr_buf) {
								txfp = 0;
								waitForOgmToComplete = TRUE;
								bypassRead = FALSE;
							}
						}
					}
				} 
				else /* we couldn't send, might be a head-head send so ... */ 
				{
					bypassRead = FALSE; /* enable reading if couldn't send */
				}
			}

		}

		/* We are waiting for an outgoing message to complete. We
			can't send the next message until it does complete */

		if (waitForOgmToComplete)
		{
			if (ogm_complete(&sendmsg))
			{
				pmsg_unref(txup);	/* unref the message */
				txup = 0;
				waitForOgmToComplete = FALSE;
				hdrPoolStk = 0;		/* recover all the inplace headers */
			}
		}

	/* if not sending, and not waiting for ogm to complete,
		flush anything waiting in the reentered-send queue */

		if (!txfp && txrp != txwp && !waitForOgmToComplete) {
			txup = txlist[txrp];
			if (++txrp >= sizeof(txlist)/sizeof(txlist[0]))
				txrp = 0;

			txfp = txup->m_frag->fr_link;
			txfp = txfp->fr_buf ? txfp : 0;
			if (!(txpcbp = ttpcb_find(txup->m_dst))
			|| txpcbp->tt_state != TTOPEN)
				txpcbp = topvmd;
		}

		if (pvmdebmask & PDMMESSAGE) {
			pvmlogprintf("mxfer() txfp %d gotem %d tt_rxf %d\n",
					(txfp ? 1 : 0), gotem, (topvmd->tt_rxf ? 1 : 0));
		}

		if (probe && !gotem && totfr)		/* got frags, keep reading */
			wantmore = 1;
	}

	return gotem;
}
