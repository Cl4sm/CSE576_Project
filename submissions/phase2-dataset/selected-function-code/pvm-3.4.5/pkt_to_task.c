pkt_to_task(tp, pp)
	struct task *tp;
	struct pkt *pp;
{
	if (tp->t_sock >= 0 && (tp->t_flag & TF_CONN)
#ifdef SHMEM
		&& !(tp -> t_flag & TF_SHM) /* don't add socket if a shm task */
#endif
	)
		wrk_fds_add(tp->t_sock, 2);

#if defined(IMA_PGON) || defined(IMA_I860)
	if (TIDISNODE(pp->pk_dst))
		mpp_output(tp, pp);
	else
#endif
#ifdef SHMEM
	if (((tp->t_sock < 0) && (tp->t_flag & TF_SHMCONN))
		|| (tp->t_flag & TF_SHMCONN) )
		mpp_output(tp, pp);
	else
#endif

#ifdef LocalRefragmentTest
	if (pp->pk_len + DDFRAGHDR <= pvmudpmtu) {
		LISTPUTBEFORE(tp->t_txq, pp, pk_link, pk_rlink);

	} else {
		struct pkt *pp2;
		int maxl = pvmudpmtu - DDFRAGHDR;
		char *cp = pp->pk_dat;
		int togo;
		int n;
		int ff = pp->pk_flag & FFSOM;
		int fe = pp->pk_flag & FFEOM;

		for (togo = pp->pk_len; togo > 0; togo -= n) {
			n = min(togo, maxl);
			pvmlogprintf("pkt_to_task() refrag len %d\n", n);
			pp2 = pk_new(0);
			pp2->pk_src = pp->pk_src;
			pp2->pk_dst = pp->pk_dst;
			if (n == togo)
				ff |= fe;
			pp2->pk_flag = ff | FFDAT;
			ff = 0;
			pp2->pk_enc = pp->pk_enc;
			pp2->pk_tag = pp->pk_tag;
			pp2->pk_ctx = pp->pk_ctx;
			pp2->pk_wid = pp->pk_wid;
			pp2->pk_crc = pp->pk_crc;
			pp2->pk_buf = pp->pk_buf;
			pp2->pk_max = pp->pk_max;
			pp2->pk_dat = cp;
			pp2->pk_len = n;
			da_ref(pp->pk_buf);
			cp += n;
			LISTPUTBEFORE(tp->t_txq, pp2, pk_link, pk_rlink);
		}
		pk_free(pp);
	}
#else /*LocalRefragmentTest*/
	{
	if (pvmdebmask & PDMMESSAGE)
		pvmlogprintf("pkt_to_task: queueing %x \n", pp->pk_dst);
	LISTPUTBEFORE(tp->t_txq, pp, pk_link, pk_rlink);
	}
#endif /*LocalRefragmentTest*/

	return 0;
}
