hostids_new(num, tids)
	int *num;			/* count requested, returned */
	int *tids;			/* return tids */
{
	static int lasthh = 1;
	int oldhh;
	int maxhostid = tidhmask >> (ffs(tidhmask) - 1);
	int i;

	oldhh = lasthh;

	/* find next free hostid */

	for (i = 0; i < *num; i++) {
		if (++lasthh > maxhostid)
			lasthh = 1;
		while ((lasthh <= hosts->ht_last && hosts->ht_hosts[lasthh])) {
			if (++lasthh > maxhostid)
				lasthh = 1;
			if (lasthh == oldhh)
				goto done;
		}
		tids[i] = lasthh << (ffs(tidhmask) - 1);
/*
		pvmlogprintf("hostids_new() tid %x\n", tids[i]);
*/
	}

done:
	return *num = i;
}
