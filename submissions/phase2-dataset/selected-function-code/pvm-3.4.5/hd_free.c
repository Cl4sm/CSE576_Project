void
hd_free(hp)
	struct hostd *hp;
{
	struct mca *mcap;

	if (hp->hd_name)
		PVM_FREE(hp->hd_name);
	if (hp->hd_arch)
		PVM_FREE(hp->hd_arch);
	if (hp->hd_login)
		PVM_FREE(hp->hd_login);
	if (hp->hd_dpath)
		PVM_FREE(hp->hd_dpath);
	if (hp->hd_epath)
		PVM_FREE(hp->hd_epath);
	if (hp->hd_bpath)
		PVM_FREE(hp->hd_bpath);
	if (hp->hd_wdir)
		PVM_FREE(hp->hd_wdir);
	if (hp->hd_sopts)
		PVM_FREE(hp->hd_sopts);
	if (hp->hd_txq)
		pk_free(hp->hd_txq);
	if (hp->hd_opq)
		pk_free(hp->hd_opq);
	if (hp->hd_rxq)
		pk_free(hp->hd_rxq);
	if (hp->hd_rxm)
		pmsg_unref(hp->hd_rxm);
	if (mcap = hp->hd_mcas) {
		while (mcap->mc_link != mcap)
			mca_free(mcap->mc_link);
		mca_free(mcap);
	}
	if (hp->hd_aname)
		PVM_FREE(hp->hd_aname);
	if (hp->hd_vmid)
		PVM_FREE(hp->hd_vmid);
	PVM_FREE(hp);
}
