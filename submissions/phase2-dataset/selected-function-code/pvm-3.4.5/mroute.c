mroute(mid, dtid, code, tmout)
	int mid;				/* message */
	int dtid;				/* dest */
	int code;				/* type tag */
	struct timeval *tmout;	/* get at least one message */
{
	struct pmsg *txup;			/* tx message or null */
	struct frag *txfp = 0;		/* cur tx frag or null */
	int gotem = 0;				/* count complete msgs downloaded */
	int block;					/* get at least one message */
	int loopcount = 0;
	struct msgboxhdr *inbp;		/* incoming box */
	struct timeval tnow, tstop;
	int sbf;					/* reply to control message */
	int cc;
	int freethis = 0;			/* (control) message came from stack */
	int tstkp = 0;
	int tstk[100];				/* XXX shouldn't be a stack */

	/* XXX do we really have to do this? */
/* 	if ((dtid == TIDPVMD && code == TM_MCA) || dtid == TIDGID) */
/* 		return node_mcast(mid, dtid, code); */

	if (tmout) {
		if (tmout->tv_sec || tmout->tv_usec) {
			pvmgetclock(&tnow);
			tstop.tv_sec = tnow.tv_sec + tmout->tv_sec;
			tstop.tv_usec = tnow.tv_usec + tmout->tv_usec;
			block = 1;
		} else
			block = 0;
	} else {
		block = 1;
		tstop.tv_sec = -1;
		tstop.tv_usec = -1;
	}

	if (txup = midtobuf(mid)) {
		txfp = txup->m_frag->fr_link;
		txfp = txfp->fr_buf ? txfp : 0;
	}

	inbp = (struct msgboxhdr *)pvminbox;

	do {
		if (block && tstop.tv_sec != -1) {
			pvmgetclock(&tnow);
			if (tnow.tv_sec > tstop.tv_sec
			|| (tnow.tv_sec == tstop.tv_sec && tnow.tv_usec >= tstop.tv_usec))
				break;
		}

		if (pvmpolltype == PvmPollSleep
		&& loopcount++ > pvmpolltime && !txfp && tstop.tv_sec == -1) {
			PAGELOCK(&inbp->mb_lock);
			if (inbp->mb_read == inbp->mb_last) {
				inbp->mb_sleep = 1;
				PAGEUNLOCK(&inbp->mb_lock);
				if (peer_wait() == -1)
					return PvmSysErr;
				loopcount = 0;
			} else
				PAGEUNLOCK(&inbp->mb_lock);
		}

		if (inbp->mb_read != inbp->mb_last) {
			if ((sbf = peer_recv(&gotem)) == -1)
				return PvmSysErr;
			if (sbf > 0) {
				if (txfp)
					tstk[tstkp++] = sbf;
				else {
					txup = midtobuf(sbf);
					dtid = txup->m_dst;
					code = txup->m_tag;
					txfp = txup->m_frag->fr_link;
					txfp = txfp->fr_buf ? txfp : 0;
					freethis = 1;
				}
			}
		}

		if (txfp) {
			if ((cc = peer_send(txup, txfp, dtid, code)) < 0)
				return cc;
			if (cc) {
				txfp = txfp->fr_link;
				if (!txfp->fr_buf) {
					if (freethis)
						umbuf_free(txup);
					if (tstkp > 0) {
						txup = midtobuf(tstk[--tstkp]);
						dtid = txup->m_dst;
						code = txup->m_tag;
						txfp = txup->m_frag->fr_link;
						txfp = txfp->fr_buf ? txfp : 0;
						freethis = 1;
					} else
						txfp = 0;
				}
			}
		}

	} while (txfp || (block && !gotem));

	return gotem;
}
