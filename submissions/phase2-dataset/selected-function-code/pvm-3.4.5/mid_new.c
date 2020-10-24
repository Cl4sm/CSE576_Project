static int
mid_new(mp)
	struct pmsg *mp;
{
	static int nxtmidhsiz;
	int mid;
	int ns;
	struct mhp *newh;

	/* check free list empty */

	if (!pvmmidhfree) {

	/* extend midh array */

		if (pvmmidhsiz) {
			ns = nxtmidhsiz;
			if (!(newh = TREALLOC(pvmmidh, ns, struct mhp)))
				return PvmNoMem;
			nxtmidhsiz += pvmmidhsiz;

		} else {
			nxtmidhsiz = 13;
			ns = 8;
			if (!(newh = TALLOC(ns, struct mhp, "mid")))
				return PvmNoMem;
		}
		pvmmidh = newh;

	/* put new entries on free list */

		while (pvmmidhsiz < ns) {
			pvmmidh[pvmmidhsiz].m_umb = 0;
			pvmmidh[pvmmidhsiz].m_free = pvmmidhfree;
			pvmmidhfree = pvmmidhsiz++;
		}
	}

	/* take next from free list */

	mid = pvmmidhfree;
	pvmmidhfree = pvmmidh[pvmmidhfree].m_free;

	pvmmidh[mid].m_umb = mp;
	mp->m_mid = mid;
	return mid;
}
