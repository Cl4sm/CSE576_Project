static struct frag *
frag_get_header()
{
	struct frag *fp;
	int n;

#ifdef	CLUMP_ALLOC
	if (numfrags == 0) {
		freefrags.fr_link = freefrags.fr_rlink = &freefrags;
		if (!(fp = TALLOC(FRAG_CLUMP, struct frag, "frgs")))
			return (struct frag *)0;
		for (n = FRAG_CLUMP; n-- > 0; ) {
			LISTPUTBEFORE(&freefrags, fp, fr_link, fr_rlink);
			fp++;
		}
		numfrags = FRAG_CLUMP;
	}
	numfrags--;
	fp = freefrags.fr_link;
	LISTDELETE(fp, fr_link, fr_rlink);

#else
	fp = TALLOC(1, struct frag, "frag");
#endif

	return fp;
}
