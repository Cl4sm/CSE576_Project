void
hd_dump(hp)
	struct hostd *hp;
{
	int n;
	struct pkt *pp, *pp2;

	pvmlogprintf(
			" hd_dump() ref %d t 0x%x n \"%s\" a \"%s\" ar \"%s\" dsig 0x%x\n",
			hp->hd_ref,
			hp->hd_hostpart,
			(hp->hd_name ? hp->hd_name : ""),
			(hp->hd_aname ? hp->hd_aname : ""),
			(hp->hd_arch ? hp->hd_arch : ""),
			hp->hd_dsig);

	pvmlogprintf(
			"           lo \"%s\" so \"%s\" dx \"%s\" ep \"%s\" bx \"%s\" wd \"%s\" sp %d\n",
			(hp->hd_login ? hp->hd_login : ""),
			(hp->hd_sopts ? hp->hd_sopts : ""),
			(hp->hd_dpath ? hp->hd_dpath : ""),
			(hp->hd_epath ? hp->hd_epath : ""),
			(hp->hd_bpath ? hp->hd_bpath : ""),
			(hp->hd_wdir ? hp->hd_wdir : ""),
			hp->hd_speed);

	n = 0;
	if (pp2 = hp->hd_txq)
		for (pp = pp2->pk_link; pp != pp2; pp = pp->pk_link)
			n++;
	pvmlogprintf("           sa %s mtu %d f 0x%x e %d txq %d\n",
			inadport_decimal(&hp->hd_sad),
			hp->hd_mtu,
			hp->hd_flag,
			hp->hd_err,
			n);

	pvmlogprintf(
			"           tx %d rx %d rtt %d.%06d id \"%s\"\n",
			hp->hd_txseq, hp->hd_rxseq,
			hp->hd_rtt.tv_sec, hp->hd_rtt.tv_usec,
			(hp->hd_vmid ? hp->hd_vmid : ""));
}
