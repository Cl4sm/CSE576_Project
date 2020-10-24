sendmessage(mp)
	struct pmsg *mp;
{
	struct hostd *hp = 0;
	struct task *tp;
	struct frag *fp;
	struct pkt *pp;
	int ff = FFSOM;
	int dst = mp->m_dst;

	if (!dst) {
		pvmlogerror("sendmessage() what? to t0\n");
	}

	if (pvmdebmask & PDMMESSAGE) {
		pvmlogprintf("sendmessage() dst t%x ctx %d tag %s len %d\n",
				dst, mp->m_ctx, pvmnametag(mp->m_tag, (int *)0), mp->m_len);
	}

	/*
	*	add a frag to empty message to simplify handling
	*/

	if ((fp = mp->m_frag->fr_link) == mp->m_frag) {
		fp = fr_new(MAXHDR);
		fp->fr_dat += MAXHDR;
		LISTPUTBEFORE(mp->m_frag, fp, fr_link, fr_rlink);
	}

	/*
	*	route message
	*/

	if (!(dst & tidhmask) || (dst & tidhmask) == myhostpart) {	/* to local */

		if (TIDISTASK(dst)) {				/* to local task */

			if (tp = task_find(dst)) {
				mesg_to_task(tp, mp);

			} else
				if (pvmdebmask & (PDMMESSAGE|PDMAPPL)) {
					pvmlogprintf(
							"sendmessage() scrapped, no such task t%x\n",
							dst);
				}

		} else {				/* to myself */
			mp->m_ref++;
			mesg_rewind(mp);
			netentry(hosts->ht_hosts[hosts->ht_local], mp);
		}

	} else {					/* to remote */

	/* lookup host */

		if (runstate == PVMDHTUPD)
			hp = tidtohost(newhosts, dst);
		if (!hp && !(hp = tidtohost(hosts, dst))) {
			if (pvmdebmask & (PDMMESSAGE|PDMAPPL)) {
				pvmlogprintf("sendmessage() scrapped, no such host t%x\n",
						dst);
			}
			goto bail;
		}

	/* packetize frags */

		do {
			pp = pk_new(0);
			if (ff & FFSOM) {
				pp->pk_enc = mp->m_enc;
				pp->pk_tag = mp->m_tag;
				pp->pk_ctx = mp->m_ctx;
				pp->pk_wid = mp->m_wid;
#ifdef	MCHECKSUM
				pp->pk_crc = mesg_crc(mp);
#else
				pp->pk_crc = 0;
#endif
			}
			pp->pk_buf = fp->fr_buf;
			pp->pk_dat = fp->fr_dat;
			pp->pk_max = fp->fr_max;
			pp->pk_len = fp->fr_len;
			da_ref(pp->pk_buf);
			if (fp->fr_link == mp->m_frag)
				ff |= FFEOM;
			pp->pk_src = mp->m_src;
			pp->pk_dst = dst;
			pp->pk_flag = ff;
			ff = 0;
			if (mp->m_flag & MM_PRIO) {
				if (pvmdebmask & (PDMMESSAGE|PDMAPPL))
					pvmlogerror("sendmessage() PRIO message to host? (scrapped)\n");

			} else {
				pkt_to_host(hp, pp);
			}
		} while ((fp = fp->fr_link) != mp->m_frag);
	}

bail:
	pmsg_unref(mp);
	return 0;
}
