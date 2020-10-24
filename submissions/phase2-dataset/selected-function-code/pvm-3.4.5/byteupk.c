static int
byteupk(mp, cp, num, siz, lnc)
	struct pmsg *mp;
	char *cp;			/* base of data */
	int num;			/* num of chunks */
	int siz;			/* size of chunk */
	int lnc;			/* lead to next chunk */
{
	struct frag *fp;			/* working frag */
	int togo;					/* bytes left in chunk */
	int r;						/* bytes (data) left in frag */
#ifdef IMA_CSPP
	int diff_node = mp->m_flag & MM_DIFFNODE;
#endif

	if (siz == lnc) {		/* if contiguous, treat as single chunk */
		lnc = (siz *= num);
		num = 1;
	}
	lnc -= siz;		/* now bytes between chunks */

	while (num-- > 0) {		/* copy chunks until done */

		for (togo = siz; togo > 0; ) {
			fp = mp->m_cfrag;
			r = fp->fr_len - mp->m_cpos;

			if (togo <= r) {	/* frag contains rest of chunk */
#ifdef IMA_CSPP
				fp->fr_num_unpacked += togo;
				if (diff_node) {
					pre_bcopy((unsigned int)(fp->fr_dat + mp->m_cpos),
							(unsigned int)cp, togo);
				} else
#endif
				BCOPY(fp->fr_dat + mp->m_cpos, cp, togo);
				mp->m_cpos += togo;
				cp += togo;
				togo = 0;

			} else {
				if (r > 0) {	/* frag contains part of chunk */
#ifdef IMA_CSPP
					fp->fr_num_unpacked += r;
					if (diff_node) {
						pre_bcopy((unsigned int)(fp->fr_dat + mp->m_cpos),
								(unsigned int)cp, r);
					} else
#endif
					BCOPY(fp->fr_dat + mp->m_cpos, cp, r);
					mp->m_cpos += r;
					togo -= r;
					cp += r;

				} else {		/* no space, add new frag */
					if (r = pmsg_decmore(mp))
						return r;
				}
			}
		}
		cp += lnc;
	}
	return 0;
}
