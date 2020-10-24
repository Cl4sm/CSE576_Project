ht_delete(htp, hp)
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
	if (hh < 0 || hh > htp->ht_last || htp->ht_hosts[hh] != hp) {
		pvmlogerror("ht_delete() host not in table\n");
		return;
	}
	htp->ht_hosts[hh] = 0;
	if (hh)
		htp->ht_cnt--;

	hd_unref(hp);

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
