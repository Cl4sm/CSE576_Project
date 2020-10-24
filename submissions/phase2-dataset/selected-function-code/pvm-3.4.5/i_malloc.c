char*
i_malloc(len, tag)
	unsigned len;	/* number of bytes */
	char *tag;		/* content description */
{
	char *ptr;
	struct glob *ob;	/* hash tbl entry */
	struct glob **he;
	int i;				/* gp */

	if (firsttime) {
		firsttime = 0;
		BZERO((char*)hashtbl, sizeof(hashtbl));

#if	STATICGLOBS > 0
	/* initialize the glob freelist */
		ob = 0;
		for (i = STATICGLOBS-1; i >= 0; i--) {
			globheap[i].next = ob;
			ob = &globheap[i];
		}
		globfl = ob;
#endif
	}

	/* check req length */

#if LET0BE1 > 0
	if (!len)
		len = 1;
#endif
	if (len < 1 || len > lengthlimit) {
		(void)sprintf(msbuf, "i_malloc: bogus len=%d\n", len);
		(void)SWRITE(debfd, msbuf);
		i_choke();
		return (char*)0;
	}

	/* do actual malloc */

	if (!(ptr = (char*)malloc(len + lopad + hipad))) {
		(void)sprintf(msbuf, "i_malloc: malloc failed len=%d\n", len);
		(void)SWRITE(debfd, msbuf);
		i_choke();
		return (char*)0;
	}

	/* get descriptor */

#if STATICGLOBS > 0
	if (ob = globfl) {
		globfl = globfl->next;
		globavail--;

	} else {
		(void)sprintf(msbuf, "i_malloc: glob allocate failed (max %d)\n",
			STATICGLOBS);
		(void)SWRITE(debfd, msbuf);
		i_choke();
		return (char*)0;
	}
#else
	if (!(ob = (struct glob*)malloc(sizeof(struct glob)))) {
		(void)sprintf(msbuf, "i_malloc: malloc failed for glob\n");
		(void)SWRITE(debfd, msbuf);
		i_choke();
		return (char*)0;
	}
#endif

	/* enter descriptor, write head and tail pads */

	ob->flg = OBALLOC;
	ob->id = ++globid;
	ob->tag[0] = 0;
	if (tag)
		strncpy(ob->tag, tag, 4);
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
	totlnbyts += len;
	return ob->base;
}
