void
ht_insert(htp, hp)
	struct htab *htp;
	struct hostd *hp;
{
	int hh;
	int i;
	unsigned long mask = 0, tmpmask;

	int *dsigs;
	int ndsigs;
	int found;
	int d;

	hh = (hp->hd_hostpart & tidhmask) >> (ffs(tidhmask) - 1);

	/* extend ht_hosts[] if no room */

	if (hh > htp->ht_last) {
		int n = htp->ht_last;

		htp->ht_last = (hh * 3) / 2;
		htp->ht_hosts = TREALLOC(htp->ht_hosts, htp->ht_last + 1, struct hostd*);
		while (++n <= htp->ht_last)
			htp->ht_hosts[n] = 0;
	}

	/* if already have an entry, take this as an update XXX kind of a hack */

	if (htp->ht_hosts[hh]) {	/* already have an entry */

		struct hostd *hp2 = htp->ht_hosts[hh];

		if (hp->hd_name) {
			if (hp2->hd_name)
				PVM_FREE(hp2->hd_name);
			hp2->hd_name = STRALLOC(hp->hd_name);
		}
		if (hp->hd_arch) {
			if (hp2->hd_arch)
				PVM_FREE(hp2->hd_arch);
			hp2->hd_arch = STRALLOC(hp->hd_arch);
		}
		hp2->hd_dsig = hp->hd_dsig;
		hp2->hd_mtu = hp->hd_mtu;
		hp2->hd_sad = hp->hd_sad;
		hp2->hd_speed = hp->hd_speed;

	} else {					/* add new entry */

		htp->ht_hosts[hh] = hp;
		if (hh)
			htp->ht_cnt++;
		hp->hd_ref++;
	}

	/* update number of arches */

	dsigs = TALLOC( htp->ht_cnt, int, "dsigs");
	ndsigs = 0;

	htp->ht_narch = 0;
	for (hh = htp->ht_last; hh > 0; hh--) {
		hp = htp->ht_hosts[hh];
		if (hp && hp->hd_arch) {
			d = htp->ht_hosts[hh]->hd_dsig;
			for ( i=0, found=0 ; i < ndsigs && !found ; i++ ) {
				if ( dsigs[i] == d )
					found++;
			}
			if ( !found ) {
				dsigs[ ndsigs++ ] = d;
				htp->ht_narch++;
			}
		}
	}

	PVM_FREE( dsigs );
}
