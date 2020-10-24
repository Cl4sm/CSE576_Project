fr_snew(cp, len)
	char *cp;	/* buffer */
	int len;	/* buffer size */
{
	struct frag *fp;

	if (!(fp = frag_get_header()))
		goto oops;

	fp->fr_link = fp->fr_rlink = 0;
	fp->fr_dat = fp->fr_buf = cp;
	fp->fr_max = fp->fr_len = len;
	fp->fr_u.ref = 1;
	fp->fr_u.dab = 0;
	fp->fr_u.spr = 0;
	fp->fr_rip = 0;
	return fp;

oops:
	pvmlogerror("fr_snew() can't get memory\n");
	pvmbailout(0);
	return (struct frag*)0;
}
