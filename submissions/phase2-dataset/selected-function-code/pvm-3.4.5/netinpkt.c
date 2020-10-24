	struct hostd *hp;
	struct pkt *pp;
{
	struct mca *mcap = 0;
	struct task *tp;
	struct pmsg *mp;
	struct frag *fp;
	struct pkt *pp2;
	int src = pp->pk_src;
	int dst = pp->pk_dst;
	int ff = pp->pk_flag;
	char *cp;
	int i;
	int firstmca = 1;

	if (pvmdebmask & PDMPACKET) {
		pvmlogprintf(
		"netinpkt() pkt from %s src t%x dst t%x f %s len %d\n",
				hp->hd_name, src, dst, pkt_flags(ff), pp->pk_len);
	}

	/* throw out packet if it's not for us */

	if (TIDISMCA(dst)) {
		for (mcap = hp->hd_mcas->mc_link; mcap != hp->hd_mcas;
				mcap = mcap->mc_link)
			if (mcap->mc_tid == dst)
				break;
		if (mcap == hp->hd_mcas)
			mcap = 0;
	}

	if ((dst & tidhmask) != myhostpart && !mcap) {
		if (pvmdebmask & (PDMPACKET|PDMAPPL)) {
			pvmlogprintf(
					"netinpkt() pkt from t%x for t%x scrapped (not us)\n",
					src, dst);
		}
		goto done;
	}

	if (mcap) {

		for (i = mcap->mc_ndst; i-- > 0; ) {
			dst = mcap->mc_dsts[i];
			if (tp = task_find(dst)) {		/* to local task */
				pp2 = pk_new(0);
				pp2->pk_src = src;
				pp2->pk_dst = dst;
				pp2->pk_flag = ff;

#if defined(IMA_MPP)
				if (firstmca)
				{
					pp2->pk_flag |= (FFMCA | FFMCAWH);
					firstmca = 0;
				}
				else
					pp2->pk_flag |= FFMCA;
#endif
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

				pkt_to_task(tp, pp2);

			} else
				if (pvmdebmask & (PDMPACKET|PDMAPPL)) {
					pvmlogprintf(
					"netinpkt() mc pkt from t%x for t%x scrapped (no dst)\n",
							src, dst);
				}
		}

		if (ff & FFEOM) {
			if (pvmdebmask & PDMMESSAGE) {
				pvmlogprintf("netinpkt() freed mca %x from t%x\n",
						mcap->mc_tid, hp->hd_name);
			}
			mca_free(mcap);
		}
		goto done;
	}

	if ((dst & ~tidhmask) == TIDPVMD) {		/* for pvmd */
		if (ff & FFSOM) {			/* start of message */
			if (hp->hd_rxm) {
				pvmlogprintf("netinpkt() repeated start pkt from %s\n",
						hp->hd_name);
				goto done;
			}
			hp->hd_rxm = mesg_new(0);
			hp->hd_rxm->m_enc = pp->pk_enc;
			hp->hd_rxm->m_tag = pp->pk_tag;
			hp->hd_rxm->m_ctx = pp->pk_ctx;
			hp->hd_rxm->m_wid = pp->pk_wid;
			hp->hd_rxm->m_crc = pp->pk_crc;
			hp->hd_rxm->m_dst = dst;
			hp->hd_rxm->m_src = src;

		} else {					/* middle or end of message */
			if (!hp->hd_rxm) {
				pvmlogprintf(
						"netinpkt() spurious pkt (no message) from %s\n",
						hp->hd_name);
				goto done;
			}
		}

		fp = fr_new(0);
		fp->fr_buf = pp->pk_buf;
		fp->fr_dat = pp->pk_dat;
		fp->fr_max = pp->pk_max;
		fp->fr_len = pp->pk_len;
		da_ref(pp->pk_buf);
		LISTPUTBEFORE(hp->hd_rxm->m_frag, fp, fr_link, fr_rlink);
		hp->hd_rxm->m_len += fp->fr_len;

		if (ff & FFEOM) {		/* end of message */
			mp = hp->hd_rxm;
			hp->hd_rxm = 0;
#ifdef	MCHECKSUM
			if (mp->m_crc != mesg_crc(mp)) {
				pvmlogprintf(
						"netinpkt() message from t%x to t%x bad checksum\n",
						src, dst);
	/* XXX must free message? */
				goto done;
			}
#endif
			mesg_rewind(mp);
			if (TIDISTASK(src)) {
				if (src == pvmschedtid) {
					schentry(mp);

				} else
					if (pvmdebmask & (PDMMESSAGE|PDMAPPL)) {
						pvmlogprintf(
						"netinpkt() mesg from t%x to t%x tag %d scrapped\n",
								src, dst, mp->m_tag);
	/* XXX must free message? */
					}

			} else {
				netentry(hp, mp);
			}
		}

	} else {								/* for a task */
		if (tp = task_find(dst)) {

#if defined(IMA_PGON) || defined(IMA_I860)
			if (TIDISNODE(dst))
				mpp_output(tp, pp);
			else
#endif
				pkt_to_task(tp, pp);
			pp = 0;

		} else {
			if (pvmdebmask & (PDMPACKET|PDMAPPL)) {
				pvmlogprintf(
						"netinpkt() pkt from t%x for t%x scrapped (no dst)\n",
						src, dst);
	/* XXX must free message? */
			}
			goto done;
		}
	}

done:
	if (pp)
		pk_free(pp);
	return 0;
}
