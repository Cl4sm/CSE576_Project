static int
mxinput(pcbp, nfr)
	struct ttpcb *pcbp;
	int *nfr;				/* number of frags read */
{
	int gotem = 0;			/* num msgs added to pvmrxlist */
	struct frag *fp;		/* shadows pcbp->tt_rxf */
	struct frag *fp2;
	int n;					/* bytes received */
	int m;					/* length of fragment */
	struct pmsg *rxup;		/* message containing this frag */
	struct pmsg *hdfrgpile;
	char *cp;				/* gp */
	int src;
	int dst;
	int ff;
	static int fairprobe = 0;

	*nfr = 0;


#if defined(IMA_MPP)

	/* Messages from Tasks and  Messages from the daemon  are probed
	 * fairprobe makes sure that neither interface gets starved.  
	 * fairprobe is toggled when an EOM is found  
	*/

	switch (fairprobe)
	{
		/* pvm_readfrom<xxxx> returns a pointer to a frag, with its
		 *	length properly filled in. cp returns a pointer to the
		 *	the start of the data
		*/
		case 0:
			if ((fp = pvm_readfrompeer()) == (struct frag *) NULL) 
				fp = pvm_readfrompvmd();
			break;
		
		default:	
			if ((fp = pvm_readfrompvmd()) == (struct frag *) NULL) 
				fp = pvm_readfrompeer();
			break;

	}

	if (!fp) 
		return gotem;	/* didn't read anything */

#else
	if (!pcbp->tt_rxf)
		if (!(pcbp->tt_rxf = fr_new(pvmfrgsiz)))
			return PvmNoMem;
	fp = pcbp->tt_rxf;

	/* read fragment header separately from body */

	n = (fp->fr_len < TDFRAGHDR) ? 0 : pvmget32(fp->fr_dat + 8);
	n += TDFRAGHDR - fp->fr_len;
	if (pvmdebmask & PDMPACKET) {
		pvmlogprintf("mxinput() pcb t%x fr_len=%d fr_dat=+%d n=%d\n",
				pcbp->tt_tid, fp->fr_len, fp->fr_dat - fp->fr_buf, n);
	}
#ifndef WIN32
	n = read(pcbp->tt_fd, fp->fr_dat + fp->fr_len, n);
#else
	n = win32_read_socket( pcbp->tt_fd,fp->fr_dat+fp->fr_len,n);
#endif
	if (pvmdebmask & PDMPACKET) {
		pvmlogprintf("mxinput() read=%d\n", n);
	}

	/* deal with errors and closes */

	if (n == -1 && 
#ifndef WIN32
			errno != EWOULDBLOCK && 
#endif 
			errno != EINTR)
	{
		if (pvmdebmask & PDMPACKET) {
			pvmlogperror("mxinput() read");
			pvmlogprintf("mxinput() t%x\n", pcbp->tt_tid);
		}
		return PvmSysErr;
	}
	if (!n) {
		if (pvmdebmask & PDMPACKET) {
			pvmlogprintf("mxinput() t%x read EOF\n", pcbp->tt_tid);
		}
		return -1;
	}

	if (n < 1)
		return gotem;
	if ((fp->fr_len += n) < TDFRAGHDR)
		return gotem;

	/* realloc buffer if frag won't fit */

	m = TDFRAGHDR + pvmget32(fp->fr_dat + 8);
	if (fp->fr_len == TDFRAGHDR) {
		if (m > fp->fr_max - (fp->fr_dat - fp->fr_buf)) {
			fp2 = fr_new(m);
			BCOPY(fp->fr_dat, fp2->fr_dat, TDFRAGHDR);
			fp2->fr_len = fp->fr_len;
			fr_unref(fp);
			fp = pcbp->tt_rxf = fp2;
			if (pvmdebmask & PDMPACKET) {
				pvmlogprintf("mxinput() realloc frag max=%d\n", m);
			}
		}
	}

#endif /* defined (IMA_MPP) */
	
	/* Now get information from the frag header to determine
	 * src, destination, length, etc.
	*/
#if !defined(IMA_MPP)
	if (fp->fr_len == m) 
#else
	if (fp)  /* got a frag */
#endif
	{
		(*nfr)++;
#if !defined (IMA_MPP)
		pcbp->tt_rxf = 0;
#endif
		cp = fp->fr_dat;
		dst = pvmget32(cp);
		src = pvmget32(cp + 4);
		ff = pvmget8(cp + 12);
		fp->fr_len -= TDFRAGHDR;
		fp->fr_dat += TDFRAGHDR;

		if (pvmdebmask & PDMPACKET) {
			pvmlogprintf("mxinput() pkt src t%x len %d ff %d\n",
					src, fp->fr_len, ff);
	}

#if defined(IMA_MPP)
	if (fp -> fr_rip )	/* frag was received inplace */
	{
		gotem++;
		fr_unref(fp); /* free frag -- mppprecv knows that the msg completed */
		return gotem;
	}

	hdfrgpile = pvm_mpp_pmsgs();
#else
	hdfrgpile = pcbp->tt_rxfrag ;
#endif
	/*
	* if start of message, make new umbuf, add to frag pile
	*/
		if (ff & FFSOM) {
			cp += TDFRAGHDR;
			fp->fr_len -= MSGHDRLEN;
			fp->fr_dat += MSGHDRLEN;
			rxup = umbuf_new();
			rxup->m_enc = pvmget32(cp);
			rxup->m_tag = pvmget32(cp + 4);
			rxup->m_ctx = pvmget32(cp + 8);
			rxup->m_wid = pvmget32(cp + 16);
			rxup->m_crc = pvmget32(cp + 20);
			rxup->m_src = src;
			rxup->m_dst = dst;

			LISTPUTBEFORE(hdfrgpile, rxup, m_link, m_rlink);

		} else {
			/* locate frag's message */

			for (rxup = hdfrgpile->m_link; rxup != hdfrgpile; 
					rxup = rxup->m_link)
				if (rxup->m_src == src)
					break;
		}

		if (rxup == hdfrgpile) {	/* uh oh, no message for it */
			pvmlogerror("mxinput() frag with no message\n");
			fr_unref(fp);

		} else {
			LISTPUTBEFORE(rxup->m_frag, fp, fr_link, fr_rlink);
			rxup->m_len += fp->fr_len;
	/*
	* if end of message, move to pvmrxlist and count it
	*/
			if (ff & FFEOM) {
#if defined(IMA_MPP)
				fairprobe = (fairprobe ? 0 : 1);
#endif
				LISTDELETE(rxup, m_link, m_rlink);
				if (pvmdebmask & PDMMESSAGE) {
					pvmlogprintf(
							"mxinput() src t%x route t%x ctx %d tag %s len %d\n",
							rxup->m_src, (pcbp != 0 ? pcbp->tt_tid: -1), 
							rxup->m_ctx,
							pvmnametag(rxup->m_tag, (int *)0), rxup->m_len);
				}
#ifdef	MCHECKSUM
				if (rxup->m_crc != umbuf_crc(rxup)) {
					pvmlogprintf(
					"mxinput() message src t%x route t%x tag %s bad checksum\n",
							rxup->m_src, pcbp->tt_tid,
							pvmnametag(rxup->m_tag, (int *)0));
					umbuf_free(rxup);

				} else {
#endif
					pmsg_setenc(rxup, rxup->m_enc);
					mesg_input(rxup);
					gotem++;
#ifdef	MCHECKSUM
				}
#endif
			}
		}
	}
	return gotem;
}
