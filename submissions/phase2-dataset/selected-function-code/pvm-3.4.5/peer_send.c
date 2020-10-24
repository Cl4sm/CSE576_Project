int
peer_send(txup, txfp, dtid, code)
	struct pmsg *txup;		/* tx message or null */
	struct frag *txfp;		/* cur tx frag or null */
	int dtid;				/* dest */
	int code;				/* type code */
{
	char *cp = 0;				/* points to copy-databuf (if necessary) */
	int ff;
	int loc;					/* location of data in shared segment */
	int next;					/* frag being received */
	struct peer *pp;
	struct shmpkhdr *dmsgs;
	struct msgboxhdr *dboxp;	/* receiving box of peer */

	if (!txfp->fr_u.dab) {
		pvmlogerror("peer_send() PvmDataInPlace not implemented\n");
		return PvmNotImpl;
	}

	/* If its a control message always pass via the daemon */
	/* This avoids the I'cant send to you as you haven't handled my */
	/* tc_shmat handshake *message* yet problem... it was the chicken first */
	/* why? because the daemon is connected as beatask() has worked (maybe) */

	if (((dtid & pvmtidhmask) == (pvmmytid & pvmtidhmask)
	&& (dtid & ~pvmtidhmask) != TIDPVMD) /* to local task */
	&& !(code>=TC_FIRST && code<=TC_LAST) )	/* and not a CTRL TC message */
	{
		int new_connection;

		if (!(pp = peer_conn(dtid, &new_connection)))
			return 0;
		if (pp != (struct peer *)-1L) {
			dboxp = (struct msgboxhdr *)pp->p_buf;
			if (new_connection) {
				struct pmsg *mp;
				int sbf, l;
				struct msgboxhdr *inbp = (struct msgboxhdr *)pvminbox;
				static struct timeval ztv = { 0, 0 };

				sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
				l = TDPROTOCOL;
				pvm_pkint(&l, 1, 1);
				sbf = pvm_setsbuf(sbf);
				inbp->mb_attached = 0;
	/* XXX glggg, could we just return to mroute here instead of calling it? */
				mp = midtobuf(sbf);
				mp->m_ctx = SYSCTX_TC;
				mroute(sbf, dtid, TC_SHMAT, &ztv);
				pvm_freebuf(sbf);
 				while (!inbp->mb_attached)  
					mroute(0, 0, 0, &ztv);		/* avoid deadlock */
				if (inbp->mb_attached != dtid) {
					sprintf(pvmtxt,
						"peer_send: mb_attached: expected %x, got %x\n",
						dtid, inbp->mb_attached);
					pvmlogerror(pvmtxt);
				}
			}

		} else
			dboxp = (struct msgboxhdr *)pvmdinbox;
	} else
		dboxp = (struct msgboxhdr *)pvmdinbox;

	if ((dboxp->mb_last + 1) % pvminboxsz == dboxp->mb_read)
		return 0;		/* full */

	/*
	* if page is private, copy and replace it with one in shared buf
	*/

	if ((loc = txfp->fr_dat - outmsgbuf) > outbufsz * pvmpgsz || loc < 0) {
		if (nbufsowned == outbufsz) {
			int i;

			pvmlogerror("peer_send() Message(s) too long for shared buffer, deadlocked.\n");
			PAGELOCK(&((struct shmpghdr *)infopage)->pg_lock);
			for (i = 0; i < maxpidtid; i++)
				if (pidtids[i].pt_tid == pvmmytid) {
					pidtids[i].pt_cond = 1;
					break;
				}
			PAGEUNLOCK(&((struct shmpghdr *)infopage)->pg_lock);
		}

		cp = 0;
		do {
			if (cp)
				da_unref(cp);
			cp = da_new(MAXHDR + txfp->fr_len);
		} while ((loc = cp - outmsgbuf + MAXHDR) > outbufsz*pvmpgsz || loc < 0);

		BCOPY(txfp->fr_dat, cp + MAXHDR, txfp->fr_len);
		txfp->fr_dat = cp + MAXHDR;
		da_unref(txfp->fr_buf);
		txfp->fr_buf = cp;
	}

	if (txfp->fr_rlink == txup->m_frag)
		ff = FFSOM;
	else
		ff = 0;
	if (txfp->fr_link == txup->m_frag)
		ff |= FFEOM;

	if (pvmdebmask & PDMPACKET) {
		sprintf(pvmtxt, "peer_send() dst t%x len %d page %d flag %d\n",
			dtid, txfp->fr_len, loc/pgsz + 1, ff);
		pvmlogerror(pvmtxt);
	}

	dmsgs = (struct shmpkhdr *)(dboxp + 1);
	PAGELOCK(&dboxp->mb_lock);
	next = (dboxp->mb_last + 1) % pvminboxsz;
	/* if receive buffer full, must fail here and try again - sgi:jpb */
	if ( next == dboxp->mb_read ) {		/* full */
		PAGEUNLOCK(&dboxp->mb_lock);
		return 0;
	}
	dmsgs[next].ph_src = pvmmytid;
	dmsgs[next].ph_dst = dtid;
	dmsgs[next].ph_sdr = pvmmytid;
	dmsgs[next].ph_dat = loc;
	dmsgs[next].ph_len = txfp->fr_len;
	dmsgs[next].ph_flag = ff;
	if (ff & FFSOM) {
		dmsgs[next].ph_tag = code;
		dmsgs[next].ph_ctx = txup->m_ctx;
		dmsgs[next].ph_enc = txup->m_enc;
		dmsgs[next].ph_wid = txup->m_wid;
#ifdef	MCHECKSUM
		dmsgs[next].ph_crc = umbuf_crc(txup);
#else
		dmsgs[next].ph_crc = 0;
#endif
		if (pvmdebmask & PDMMESSAGE) {
			sprintf(pvmtxt, "peer_send() dst t%x tag %s ctx %d\n",
				dtid, pvmnametag(dmsgs[next].ph_tag,(int*)0), 
				dmsgs[next].ph_ctx);
			pvmlogerror(pvmtxt);
		} /* logging */

	} else {
		dmsgs[next].ph_tag = 0;
		dmsgs[next].ph_ctx = 0;
		dmsgs[next].ph_enc = 0;
		dmsgs[next].ph_wid = 0;
		dmsgs[next].ph_crc = 0;
	}
	da_ref(txfp->fr_buf);
	dboxp->mb_last = next;

	if (dboxp != (struct msgboxhdr *)pvmdinbox && dboxp->mb_sleep) {

/* #if	defined(IMA_SUNMP) || defined(IMA_RS6KMP) || defined(IMA_AIX4MP) || defined(IMA_AIX5MP) */
#ifdef  PVMUSEMUTEX
#ifdef	IMA_SUNMP
		cond_signal(&dboxp->mb_cond);
#endif
#if defined(IMA_RS6KMP) || defined(IMA_AIX4MP) || defined(IMA_AIX5MP)
		pthread_cond_signal(&dboxp->mb_cond);
#endif
#else
		peer_wake(pp);
#endif /* PVMUSEMUTEX */
		dboxp->mb_sleep = 0;
	}

	/* wake up pvmd */

	if (dboxp == (struct msgboxhdr *)pvmdinbox
	&& (dboxp->mb_last + pvminboxsz - 1) % pvminboxsz == dboxp->mb_read) {
		PAGEUNLOCK(&dboxp->mb_lock);
		(void)prodpvmd();
	} else
		PAGEUNLOCK(&dboxp->mb_lock);

	return 1;
}
