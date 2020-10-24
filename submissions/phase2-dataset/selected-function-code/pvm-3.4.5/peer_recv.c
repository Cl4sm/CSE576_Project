int
peer_recv(gotem)
	int *gotem;
{
	struct pmsg *rxup;			/* rx message */
	struct pmsg *up;
	struct frag *fp;
	int sdr;
	int src;
	int ff;
	struct peer *pp = 0;
	int next;					/* frag being received */
	struct shmpkhdr *inmsgs;	/* incoming messages */
	struct msgboxhdr *inbp;		/* incoming box */
	int sbf = 0;				/* reply control message */

	inbp = (struct msgboxhdr *)pvminbox;
	inmsgs = (struct shmpkhdr *)(inbp + 1);

	next = (inbp->mb_read + 1) % pvminboxsz;
	sdr = inmsgs[next].ph_sdr;
	fp = fr_new(0);
	if ((sdr & ~pvmtidhmask) != TIDPVMD) {
		if (!(pp = peer_conn(sdr, (int *)0)) || pp == (struct peer *)-1L) {
			sprintf(pvmtxt, "peer_recv() can't connect to sender t%x\n", sdr);
			pvmlogerror(pvmtxt);
			return PvmSysErr;
		}
		fp->fr_dat = pp->p_buf + INBOXPAGE*pgsz + inmsgs[next].ph_dat;
	} else
		fp->fr_dat = pvmdoutbuf + inmsgs[next].ph_dat;
	fp->fr_buf = fp->fr_dat - (inmsgs[next].ph_dat & (pgsz-1))
				+ PVMPAGEHDR;
	fp->fr_max = pvmfrgsiz;
	ff = inmsgs[next].ph_flag;

	src = inmsgs[next].ph_src;
	fp->fr_len = inmsgs[next].ph_len;
	fp->fr_max = pvmfrgsiz;

	if (pvmdebmask & PDMPACKET) {
		sprintf(pvmtxt, "peer_recv() sdr t%x src t%x len %d dst t%x flag %d\n",
			sdr, src, fp->fr_len, inmsgs[next].ph_dst, ff);
		pvmlogerror(pvmtxt);
	}

	/*
	* if start of message, make new umbuf, add to frag pile
	*/
	if (ff & FFSOM) {
		rxup = umbuf_new();
		rxup->m_tag = inmsgs[next].ph_tag;
		rxup->m_ctx = inmsgs[next].ph_ctx;
		rxup->m_enc = inmsgs[next].ph_enc;
		rxup->m_wid = inmsgs[next].ph_wid;
		rxup->m_crc = inmsgs[next].ph_crc;
		rxup->m_src = src;
#ifdef IMA_CSPP
		if (pp && pp->p_node != current_node())
			rxup->m_flag |= MM_DIFFNODE;
#endif
		LISTPUTBEFORE(rxfrag, rxup, m_link, m_rlink);
	}

	/* locate frag's message */

	for (rxup = rxfrag->m_link; rxup != rxfrag; rxup = rxup->m_link)
		if (rxup->m_src == src)
			break;

	if (rxup == rxfrag) {	/* uh oh, no message for it */
		pvmlogerror("peer_recv() frag with no message\n");
		fr_unref(fp);

	} else {
		LISTPUTBEFORE(rxup->m_frag, fp, fr_link, fr_rlink);
		rxup->m_len += fp->fr_len;
	/*
	* if end of message, move to rxlist and count it
	*/
		if (ff & FFEOM) {
			LISTDELETE(rxup, m_link, m_rlink);
#ifdef	MCHECKSUM
			if (rxup->m_crc != umbuf_crc(rxup)) {
				sprintf(pvmtxt,
				"peer_recv() message src t%x tag %d bad checksum\n",
						rxup->m_src, rxup->m_tag);
				pvmlogerror(pvmtxt);
				umbuf_free(rxup);

			} else {
#endif
				pmsg_setenc(rxup, rxup->m_enc);
				mesg_input(rxup);
				(*gotem)++;
#ifdef	MCHECKSUM
			}
#endif
		}
	}
	inbp->mb_read = next;
	return sbf;
}
