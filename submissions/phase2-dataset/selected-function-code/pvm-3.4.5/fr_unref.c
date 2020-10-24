void
fr_unref(fp)
	struct frag *fp;		/* master frag */
{
/*
	pvmlogprintf("fr_unref() %lx ref %d\n", fp, fp->fr_u.ref);
*/
	if (fp->fr_u.ref-- == 1) {
		struct frag *fp2, *fp3;

		if (fp->fr_buf) {		/* slave frag */
			if (fp->fr_u.dab)
#ifdef IMA_CSPP
			if (fp->fr_num_unpacked) {
				unsigned int addr = (unsigned int)(fp->fr_dat);
				int nbytes = fp->fr_num_unpacked;

				nbytes += (addr & 0x3f);
				addr &= ~0x3f;
				dcache_flush_region(addr, nbytes);
			}
#endif
				da_unref(fp->fr_buf);

		} else {				/* master frag */

	/* unref all frags in chain */
			for (fp2 = fp->fr_link; fp2 != fp; fp2 = fp3) {
				fp3 = fp2->fr_link;
				LISTDELETE(fp2, fr_link, fr_rlink);
				fr_unref(fp2);
			}
		}
		frag_put_header(fp);
	}
}
