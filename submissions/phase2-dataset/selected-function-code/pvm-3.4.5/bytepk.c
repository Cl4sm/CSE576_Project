static int
bytepk(mp, cp, num, siz, lnc)
	struct pmsg *mp;
	char *cp;			/* base of data */
	int num;			/* num of chunks */
	int siz;			/* size of chunk */
	int lnc;			/* lead to next chunk */
{
	struct frag *fp;			/* working frag */
	int togo;					/* bytes left in chunk */
	int r;						/* bytes (space) left in frag */

	if (siz == lnc) {		/* if contiguous, treat as single chunk */
		lnc = (siz *= num);
		num = 1;
	}
	lnc -= siz;		/* now bytes between chunks */

	while (num-- > 0) {		/* copy chunks until done */

		for (togo = siz; togo > 0; ) {
			fp = mp->m_frag->fr_rlink;
			r = fp->fr_max - (fp->fr_dat - fp->fr_buf) - fp->fr_len;

			if (togo <= r) {	/* space in frag for entire chunk */
				BCOPY(cp, fp->fr_dat + fp->fr_len, togo);
				fp->fr_len += togo;
				cp += togo;
				togo = 0;

			} else {
				if (r > 0) {	/* space for part of chunk */
					BCOPY(cp, fp->fr_dat + fp->fr_len, r);
					fp->fr_len += r;
					togo -= r;
					cp += r;

				} else {		/* no space, add new frag */
					if (r = pmsg_extend(mp))
						return r;
				}
			}
		}
		cp += lnc;
	}
	return 0;
}
