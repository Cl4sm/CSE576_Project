finack_to_host(hp)
	struct hostd *hp;
{
	struct pkt *pp;

	if (pvmdebmask & PDMPACKET) {
		pvmlogprintf("finack_to_host() %s\n", hp->hd_name);
	}
	pp = pk_new(DDFRAGHDR);	/* XXX could reref a dummy databuf here */
	pp->pk_dat += DDFRAGHDR;
	pp->pk_dst = hp->hd_hostpart | TIDPVMD;
	pp->pk_src = pvmmytid;
	pp->pk_flag = FFFIN|FFACK;
	TVCLEAR(&pp->pk_rtv);
	TVCLEAR(&pp->pk_rta);
	TVCLEAR(&pp->pk_rto);
	TVCLEAR(&pp->pk_at);
	pp->pk_nrt = 0;
	pp->pk_hostd = hp;
	pp->pk_seq = 0;
	pp->pk_ack = 0;
	LISTPUTAFTER(opq, pp, pk_tlink, pk_trlink);
	return 0;
}
