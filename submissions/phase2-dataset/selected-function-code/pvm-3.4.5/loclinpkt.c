loclinpkt(tp, pp)
	struct task *tp;
	struct pkt *pp;
{
	int dst;			/* pkt dst */
	int ff;				/* pkt flags */
	struct pkt *pp2;
	struct frag *fp;
	struct pmsg *mp;
	struct hostd *hp;
	struct task *tp2;
#if defined(IMA_CM5) || defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
		|| defined(IMA_AIX5SP2)
	struct task *socktp = tp;	/* owner of the socket */
#endif

	dst = pp->pk_dst;
	ff = pp->pk_flag;
	if (pvmdebmask & PDMPACKET) {
		pvmlogprintf(
				"loclinpkt() src t%x dst t%x f %s len %d\n",
				pp->pk_src, dst, pkt_flags(ff), pp->pk_len);
	}

#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	if (pp->pk_src > 0 && !tp->t_tid && (tp2 = task_findpid(pp->pk_src))) {
		/* connect request from pvmhost */
		mpp_conn(tp, tp2);
		pk_free(pp);
		return -1;
	}
#endif
#if defined(IMA_PGON) || defined(IMA_I860) || defined(IMA_CM5) \
		|| defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
		|| defined(IMA_AIX5SP2)
	if (TIDISNODE(pp->pk_src))		/* from a node */
		if (!(tp = task_find(pp->pk_src))) {
			pvmlogprintf("loclinpkt() from unknown task t%x\n", pp->pk_src);
			goto done;
		}
#endif	/*defined(IMA_PGON) || defined(IMA_I860) || defined(IMA_CM5) || defined(IMA_SP2MPI)*/

	/*
	* if to multicast addr, replicate pkt in each q
	*/

	if (TIDISMCA(dst) && tp->t_mca && tp->t_mca->mc_tid == dst) {

		struct mca *mcap = tp->t_mca;
		int i;

		for (i = mcap->mc_ndst; i-- > 0; ) {
			dst = mcap->mc_dsts[i];
			if (hp = tidtohost(hosts, dst)) {
				pp2 = pk_new(0);
				pp2->pk_src = pp->pk_src;
				pp2->pk_dst = mcap->mc_tid;
				pp2->pk_flag = ff;
				pp2->pk_enc = pp->pk_enc;
				pp2->pk_tag = pp->pk_tag;
				pp2->pk_ctx = pp->pk_ctx;
				pp2->pk_wid = pp->pk_wid;
				pp2->pk_crc = pp->pk_crc;
				pp2->pk_buf = pp->pk_buf;
				pp2->pk_max = pp->pk_max;
				pp2->pk_dat = pp->pk_dat;
				pp2->pk_len = pp->pk_len;
				da_ref(pp->pk_buf);

				if (hp->hd_hostpart == myhostpart) {
					netinpkt(hp, pp2);

				} else {
					pkt_to_host(hp, pp2);
				}

			} else
				if (pvmdebmask & (PDMPACKET|PDMAPPL)) {
					pvmlogprintf(
					"loclinpkt() pkt src t%x dst t%x scrapped (no such host)\n",
							pp->pk_src, dst);
				}
		}

	/* free mca on last pkt */

		if (ff & FFEOM) {
			if (pvmdebmask & PDMMESSAGE) {
				pvmlogprintf("loclinpkt() freed mca %x for t%x\n",
						mcap->mc_tid, tp->t_tid);
			}
			mca_free(mcap);
			tp->t_mca = 0;
		}
		goto done;
	}

	/*
	* if to a pvmd, always reassemble (forward if not for us)
	*/

	if ((dst & ~tidhmask) == TIDPVMD) {
		if (ff & FFSOM) {			/* start of message */
			if (tp->t_rxm) {
				pvmlogprintf("loclinpkt() repeated start pkt t%x\n",
						tp->t_tid);
				goto done;
			}
			tp->t_rxm = mesg_new(0);
			tp->t_rxm->m_ctx = pp->pk_ctx;
			tp->t_rxm->m_tag = pp->pk_tag;
			tp->t_rxm->m_enc = pp->pk_enc;
			tp->t_rxm->m_wid = pp->pk_wid;
			tp->t_rxm->m_crc = pp->pk_crc;
			tp->t_rxm->m_dst = dst;
			tp->t_rxm->m_src = tp->t_tid;

		} else {					/* middle or end of message */
			if (!tp->t_rxm) {
				pvmlogprintf(
					"loclinpkt() pkt with no message src t%x\n",
					tp->t_tid);
				goto done;
			}
		}

		fp = fr_new(0);
		fp->fr_buf = pp->pk_buf;
		fp->fr_dat = pp->pk_dat;
		fp->fr_max = pp->pk_max;
		fp->fr_len = pp->pk_len;
		da_ref(pp->pk_buf);

		LISTPUTBEFORE(tp->t_rxm->m_frag, fp, fr_link, fr_rlink);
		tp->t_rxm->m_len += fp->fr_len;

		if (ff & FFEOM) {		/* end of message */
			mp = tp->t_rxm;
			tp->t_rxm = 0;
#ifdef	MCHECKSUM
			if (mp->m_crc != mesg_crc(mp)) {
				pvmlogprintf(
						"loclinpkt() message src t%x dst t%x bad checksum\n",
						mp->m_src, dst);
				goto done;
			}
#endif
			if (!(dst & tidhmask) || (dst & tidhmask) == myhostpart) {	/* local */
				mesg_rewind(mp);
				if (mp->m_tag >= (int)SM_FIRST && mp->m_tag <= (int)SM_LAST
				&& (mp->m_src == pvmschedtid || mp->m_src == hostertid || mp->m_src == taskertid))
				{
					schentry(mp);

				} else {
					loclentry(tp, mp);
				}

			} else {		/* remote */
				if (!tp->t_tid) {
					pvmlogprintf("loclinpkt() pkt src null dst t%x\n", dst);
					goto done;
				}
				sendmessage(mp);
			}
	/*
	* if sock is -1, tm_conn2() wants us to throw out this context
	* because it's been merged into another.
	*/
#if defined(IMA_CM5) || defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
		|| defined(IMA_AIX5SP2)
			/* node procs have no socket; they use pvmhost's */
			if (socktp->t_sock == -1)
#else
			if (tp->t_sock == -1)
#endif
			{
				pk_free(pp);
				return -1;
			}
		}
		goto done;
	}

	/*
	* if to a task, put in local or remote send queue
	*/

	if (TIDISTASK(dst)) {
		if (!tp->t_tid) {
			pvmlogprintf("loclinpkt() pkt src null dst t%x\n", dst);
			goto done;
		}
		if ((dst & tidhmask) == myhostpart) {	/* local host */
			if (tp2 = task_find(dst)) {

#if defined(IMA_PGON) || defined(IMA_I860)
				if (TIDISNODE(dst))
					mpp_output(tp2, pp);
				else
#endif
					pkt_to_task(tp2, pp);
/*
				LISTPUTBEFORE(tp2->t_txq, pp, pk_link, pk_rlink);
*/
				pp = 0;

			} else
				if (pvmdebmask & (PDMPACKET|PDMAPPL)) {
					pvmlogprintf(
					"loclinpkt() pkt src t%x dst t%x scrapped (no such task)\n",
							pp->pk_src, dst);
				}

		} else {								/* remote host */
			if (hp = tidtohost(hosts, dst)) {
				pkt_to_host(hp, pp);
				pp = 0;

			} else {
				if (pvmdebmask & (PDMPACKET|PDMAPPL)) {
					pvmlogprintf(
					"loclinpkt() pkt src t%x dst t%x scrapped (no such host)\n",
							pp->pk_src, dst);
				}
				goto done;
			}
		}
	}

done:
	if (pp)
		pk_free(pp);

	return 0;
}
