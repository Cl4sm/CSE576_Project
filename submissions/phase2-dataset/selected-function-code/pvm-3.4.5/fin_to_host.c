fin_to_host(hp)
	struct hostd *hp;
{
	struct pkt *pp;

	if (pvmdebmask & PDMPACKET) {
		pvmlogprintf("fin_to_host() %s\n", hp->hd_name);
	}
	pp = pk_new(DDFRAGHDR);	/* XXX could reref a dummy databuf here */
	pp->pk_dat += DDFRAGHDR;
	pp->pk_dst = hp->hd_hostpart | TIDPVMD;
	pp->pk_src = pvmmytid;
	pp->pk_flag = FFFIN;
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
	LISTPUTAFTER(opq, pp, pk_tlink, pk_trlink);
	return 0;
}
