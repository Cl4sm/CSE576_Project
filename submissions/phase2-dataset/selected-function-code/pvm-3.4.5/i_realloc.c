char*
i_realloc(loc, len)
	char *loc;			/* old buffer */
	unsigned len;		/* length of new buffer */
{
	struct glob *ob;	/* freeing this object */
	struct glob *preob;	/* object before in chain */
	int rs;				/* reproduced random seqn */
	char *ptr = loc;	/* gp */
	struct glob **he;	/* hash tbl entry */
	int i;				/* gp */

	/* sanity check */

	if (firsttime) {
		char *s = "i_realloc: called before i_malloc?\n";
		SWRITE(debfd, s);
		i_choke();
		return (char*)0;
	}

	/* check req length */

#if LET0BE1 > 0
	if (!len)
		len = 1;
#endif
	if (len < 1 || len > lengthlimit) {
		(void)sprintf(msbuf, "i_realloc: bogus len=%d\n", len);
		(void)SWRITE(debfd, msbuf);
		i_choke();
		return (char*)0;
	}

	/* delete from hash tbl */

	he = &hashtbl[HASH(loc)];
	for (preob = 0, ob = *he; ob && ob->base != loc; preob = ob, ob = ob->next);
	if (!ob) {
		(void)sprintf(msbuf, "i_realloc: bogus loc=0x%lx\n",
			(long) loc);
		(void)SWRITE(debfd, msbuf);
		i_choke();
		return (char*)0;
	}
	rs = ob->rst;

	/* check head and tail pads */

	for (i = ob->lop, ptr -= i; i > 0; i--)
		if ((0xff & (int)(*ptr++)) != (0xff & NEXTRN(rs))) {
			(void)sprintf(msbuf, "i_realloc: scribbled in 0x%lx[%d]\n",
				(long) loc, -i);
			(void)SWRITE(debfd, msbuf);
			i_choke();
		}
	for (i = ob->hip, ptr += ob->len; i > 0; i--)
		if ((0xff & (int)(*ptr++)) != (0xff & NEXTRN(rs))) {
			(void)sprintf(msbuf,
				"i_realloc: scribbled in 0x%lx[%d+%d]\n",
				(long) loc, ob->len, ob->hip - i);
			(void)SWRITE(debfd, msbuf);
			i_choke();
		}

	/* remove descriptor */

	if (preob)
		preob->next = ob->next;
	else
		*he = ob->next;

	/* realloc */

	if (!(ptr = (char*)realloc(loc - ob->lop, len + lopad + hipad))) {
		(void)sprintf(msbuf, "i_realloc: malloc failed len=%d\n", len);
		(void)SWRITE(debfd, msbuf);
		i_choke();
		return (char*)0;
	}

	/* rewrite descriptor, write head and tail pads */

	totlnbyts += len - ob->len;
	ob->flg = OBREALLOC;
	ob->id = ++globid;	/* XXX ? */
	ob->len = len;
	ob->lop = lopad;
	ob->hip = hipad;
	ob->rst = rnstate;
	for (i = lopad; i-- > 0; *ptr++ = NEXTRN(rnstate));
	ob->base = ptr;
	he = &hashtbl[HASH(ptr)];
	for (i = hipad, ptr += len; i-- > 0; *ptr++ = NEXTRN(rnstate));
	ob->next = *he;
	*he = ob;
	return ob->base;
}
