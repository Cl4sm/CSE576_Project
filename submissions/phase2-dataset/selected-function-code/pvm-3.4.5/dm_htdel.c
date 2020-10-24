int
dm_htdel(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	int serial;
	int tid;

	if (hp != hosts->ht_hosts[hosts->ht_master]) {
		pvmlogprintf("dm_htdel() from t%x (not master)?\n", mp->m_src);
		return 0;
	}
	if (upkint(mp, &serial)) {
		pvmlogerror("dm_htdel() bad format\n");
		return 0;
	}
	if (serial != hosts->ht_serial) {
		pvmlogprintf("dm_htdel() for serial %d, current is %d?\n",
				serial, hosts->ht_serial);
		return 0;
	}
	while (!upkuint(mp, &tid)) {
		if (hp = tidtohost(hosts, tid)) {
			if (pvmdebmask & PDMHOST) {
				pvmlogprintf("dm_htdel() host %s\n", hp->hd_name);
			}
			hostfailentry(hp);
			ht_delete(hosts, hp);
			if (newhosts)
				ht_delete(newhosts, hp);
		}
	}
	return 0;
}
