int
mesg_to_task(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	struct frag *fp = mp->m_frag->fr_link;
	struct pkt *pp;
	int ff = FFSOM;			/* frag flags */
	int dst = mp->m_dst;

	if (pvmdebmask & PDMMESSAGE) {
		pvmlogprintf("mesg_to_task() dst t%x tag %s len %d\n",
				dst, pvmnametag(mp->m_tag, (int *)0), mp->m_len);
	}

	/* if nothing yet in q, add task's sock to wrk_wfds */

	if (tp->t_sock >= 0)
		wrk_fds_add(tp->t_sock, 2);

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
		pp->pk_src = TIDPVMD;
		pp->pk_dst = dst;
		pp->pk_flag = ff;
		ff = 0;
#if defined(IMA_PGON) || defined(IMA_I860)
		if (TIDISNODE(dst)) {
			mpp_output(tp, pp);
			continue;
		}
#endif
#ifdef SHMEM
		if ((tp->t_sock < 0) && ( (tp->t_flag & TF_SHMCONN)
			|| ((tp->t_flag & TF_PRESHMCONN) && (mp->m_flag & MM_PRIO)) ) )
		{
			/* If shmem and no d-t socket or fully connected shmem */
			/* use mpp routine instead of socket one */
			mpp_output(tp, pp);
			continue;
		}
#endif
		if (mp->m_flag & MM_PRIO) {
			LISTPUTAFTER(tp->t_txq, pp, pk_link, pk_rlink);
		} else {
			pkt_to_task(tp, pp);
/*
			LISTPUTBEFORE(tp->t_txq, pp, pk_link, pk_rlink);
*/
		}
	} while ((fp = fp->fr_link) != mp->m_frag);

	return 0;
}
