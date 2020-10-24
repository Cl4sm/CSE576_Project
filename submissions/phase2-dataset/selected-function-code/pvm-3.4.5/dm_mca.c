dm_mca(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	struct mca *mcap;
	int i;

	/* unpack struct mca from message */

	mcap = mca_new();
	upkuint(mp, &mcap->mc_tid);
	upkint(mp, &mcap->mc_ndst);
	mcap->mc_dsts = TALLOC(mcap->mc_ndst, int, "mcad");
	for (i = 0; i < mcap->mc_ndst; i++)
		upkuint(mp, &mcap->mc_dsts[i]);

	/* put on list of mcas of src host */

	LISTPUTBEFORE(hp->hd_mcas, mcap, mc_link, mc_rlink);

	if (pvmdebmask & PDMMESSAGE) {
		pvmlogprintf("dm_mca() mca %x from %s\n", mcap->mc_tid, hp->hd_name);
	}
	return 0;
}
