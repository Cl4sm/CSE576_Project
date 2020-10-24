pkt_to_host(hp, pp)
	struct hostd *hp;
	struct pkt *pp;
{
	int maxl = (hp->hd_mtu < pvmudpmtu ? hp->hd_mtu : pvmudpmtu) - DDFRAGHDR;
	int llim = pp->pk_flag & FFSOM ? maxl - MSGHDRLEN : maxl;

	pp->pk_flag = (pp->pk_flag & (FFSOM|FFEOM)) | FFDAT;
	if (pvmdebmask & PDMPACKET) {
		pvmlogprintf("pkt_to_host() pkt src t%x dst t%x f %s len %d\n",
				pp->pk_src, pp->pk_dst, pkt_flags(pp->pk_flag), pp->pk_len);
	}

	if (pp->pk_len <= llim) {
		LISTPUTBEFORE(hp->hd_txq, pp, pk_link, pk_rlink);

	} else {
		struct pkt *pp2;
		char *cp = pp->pk_dat;
		int togo;
		int n;
		int ff = pp->pk_flag & FFSOM;
		int fe = pp->pk_flag & FFEOM;

		for (togo = pp->pk_len; togo > 0; togo -= n) {
			n = min(togo, llim);
			if ((pvmdebmask & PDMPACKET) && togo != pp->pk_len) {
				pvmlogprintf("pkt_to_host() refrag len %d\n", n);
			}
#ifdef	STATISTICS
			stats.refrag++;
#endif
			pp2 = pk_new(0);
			pp2->pk_src = pp->pk_src;
			pp2->pk_dst = pp->pk_dst;
			if (n == togo)
				ff |= fe;
			pp2->pk_flag = ff | FFDAT;
			ff = 0;
			llim = maxl;
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
			LISTPUTBEFORE(hp->hd_txq, pp2, pk_link, pk_rlink);
		}
		pk_free(pp);
	}

	while (hp->hd_nop < nopax
	&& (hp->hd_txq->pk_link != hp->hd_txq)) {
		if (pvmdebmask & PDMPACKET) {
			pvmlogprintf("pkt_to_host() pkt to opq\n");
		}
		pp = hp->hd_txq->pk_link;
		LISTDELETE(pp, pk_link, pk_rlink);
		TVCLEAR(&pp->pk_rtv);
		TVXADDY(&pp->pk_rta, &hp->hd_rtt, &hp->hd_rtt);
		TVCLEAR(&pp->pk_rto);
		TVCLEAR(&pp->pk_at);
		pp->pk_nrt = 0;
		pp->pk_hostd = hp;
		pp->pk_seq = hp->hd_txseq;
		hp->hd_txseq = NEXTSEQNUM(hp->hd_txseq);
		pp->pk_ack = 0;
		LISTPUTBEFORE(hp->hd_opq, pp, pk_link, pk_rlink);
		hp->hd_nop++;
		LISTPUTBEFORE(opq, pp, pk_tlink, pk_trlink);
	}
	return 0;
}
