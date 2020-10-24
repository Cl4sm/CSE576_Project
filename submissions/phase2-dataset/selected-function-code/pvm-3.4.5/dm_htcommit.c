int
dm_htcommit(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	struct htab *htp;

	if (hp != hosts->ht_hosts[hosts->ht_master]) {
		pvmlogprintf("dm_htcommit() from t%x (not master)?\n",
				mp->m_src);
	}

	if (newhosts) {
		htp = hosts;
		hosts = newhosts;
		newhosts = 0;
		if (pvmdebmask & PDMHOST) {
			pvmlogprintf(
			"dm_htcommit() committing from host table serial %d to %d\n",
					htp->ht_serial, hosts->ht_serial);
		}

		gotnewhosts(hosts, htp);
		ht_free(htp);
		runstate = PVMDNORMAL;

	} else {
		pvmlogerror("dm_htcommit() no new host table pending?\n");
	}
	return 0;
}
