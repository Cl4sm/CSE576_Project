struct hostd *
hd_new(hh)
	int hh;		/* index in host table */
{
	struct hostd *hp;

	if (hp = TALLOC(1, struct hostd, "hd")) {
		BZERO((char*)hp, sizeof(struct hostd));
		hp->hd_ref = 1;
		hp->hd_hostpart = hh << (ffs(tidhmask) - 1);
		hp->hd_sad.sin_family = AF_INET;
		hp->hd_txq = pk_new(0);
		hp->hd_opq = pk_new(0);
		hp->hd_rxq = pk_new(0);
		hp->hd_txseq = 1;
		hp->hd_rxseq = 1;
		hp->hd_speed = 1000;
		hp->hd_rtt.tv_sec = 1;	/* XXX const */
		hp->hd_mcas = mca_new();
	}
	return hp;
}
