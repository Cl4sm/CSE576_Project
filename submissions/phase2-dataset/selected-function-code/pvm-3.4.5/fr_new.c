fr_new(len)
	int len;	/* (max) buffer size or 0 */
{
	struct frag *fp;

	if (!(fp = frag_get_header()))
		goto oops;

	if (len) {	/* slave frag */
		fp->fr_link = fp->fr_rlink = 0;
		if (!(fp->fr_dat = fp->fr_buf = da_new(len))) {
			frag_put_header(fp);
			goto oops;
		}
		fp->fr_max = len;

	} else {	/* master */
		fp->fr_link = fp->fr_rlink = fp;
		fp->fr_dat = fp->fr_buf = 0;
		fp->fr_max = 0;
	}
	fp->fr_len = 0;
	fp->fr_u.ref = 1;
	fp->fr_u.dab = 1;
	fp->fr_u.spr = 0;
	fp->fr_rip = 0;
#ifdef IMA_CSPP
	fp->fr_num_unpacked = 0;
#endif
/*
	pvmlogprintf("fr_new() %d = %lx\n", len, fp);
*/
	return fp;

oops:
	pvmlogerror("fr_new() can't get memory\n");
	pvmbailout(0);
	return (struct frag*)0;
}
