i_free(loc)
	char *loc;		/* ptr to buffer */
{
	struct glob *ob;	/* freeing this object */
	struct glob *preob;	/* object before in chain */
	int rs;				/* reproduced random seqn */
	char *ptr = loc;	/* gp */
	struct glob **he;	/* hash tbl entry */
	int i;				/* gp */

	/* sanity check */

	if (firsttime) {
		char *s = "i_free: called before i_malloc?\n";
		SWRITE(debfd, s);
		i_choke();
		return 0;
	}

	/* delete from hash tbl */

	he = &hashtbl[HASH(loc)];
	for (preob = 0, ob = *he; ob && ob->base != loc; preob = ob, ob = ob->next);
	if (!ob) {
		(void)sprintf(msbuf, "i_free: bogus loc=0x%lx\n", (long) loc);
		(void)SWRITE(debfd, msbuf);
		i_choke();
		return 0;
	}
	rs = ob->rst;

	/* check head and tail pads */

	for (i = ob->lop, ptr -= i; i > 0; i--)
		if ((0xff & (int)(*ptr++)) != (0xff & NEXTRN(rs))) {
			(void)sprintf(msbuf, "i_free: scribbled in 0x%lx[%d]\n",
				(long) loc, -i);
			(void)SWRITE(debfd, msbuf);
			i_choke();
		}
	for (i = ob->hip, ptr += ob->len; i > 0; i--)
		if ((0xff & (int)(*ptr++)) != (0xff & NEXTRN(rs))) {
			(void)sprintf(msbuf, "i_free: scribbled in 0x%lx[%d+%d]\n",
				(long) loc, ob->len, ob->hip - i);
			(void)SWRITE(debfd, msbuf);
			i_choke();
		}

	/* do actual free */

#if	ZEROONFREE
	BZERO(loc - ob->lop, ob->len + ob->lop + ob->hip);
#endif
	free(loc - ob->lop);
	totlnbyts -= ob->len;

	/* reclaim descriptor */

	if (preob)
		preob->next = ob->next;
	else
		*he = ob->next;
#if STATICGLOBS > 0
	ob->next = globfl;
	globfl = ob;
	globavail++;
#else
	free((char*)ob);
#endif
	return 0;
}
