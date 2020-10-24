i_dump(how)
	int how;			/* != 0 also do sanity checking */
{
	int ht;				/* hash table index */
	struct glob *ob;
	char *r;
	int i;
	int rs;				/* reproduced random seqn */
	char *ptr;			/* gp */
	int err;
	char tagstr[5];

#if	STATICGLOBS > 0
	sprintf(msbuf, "%s %d bytes total/%d globs free\n",
			DUMPHDR, totlnbyts, globavail);
	(void)SWRITE(debfd, msbuf);
#else
	sprintf(msbuf, "%s %d bytes total\n", DUMPHDR, totlnbyts);
	(void)SWRITE(debfd, msbuf);
#endif
	for (ht = 0; ht < 256; ht++) {
		for (ob = hashtbl[ht]; ob; ob = ob->next) {
			rs = ob->rst;
			ptr = ob->base;
			err = 0;
			if (how) {
				for (i = ob->lop, ptr -= i; i > 0; i--)
					if ((0xff & (int)(*ptr++)) != (0xff & NEXTRN(rs))) {
						(void)sprintf(msbuf,
							"%5d 0x%08lx[%d]: scribbled in [%d]\n",
							ob->id, (long) ob->base, ob->len, -i);
						(void)SWRITE(debfd, msbuf);
						err++;
					}
				for (i = ob->hip, ptr += ob->len; i > 0; i--)
					if ((0xff & (int)(*ptr++)) != (0xff & NEXTRN(rs))) {
						(void)sprintf(msbuf,
							"%5d 0x%08lx[%d]: scribbled in [%d+%d]\n",
							ob->id, (long) ob->base, ob->len, ob->len,
							ob->hip - i);
						(void)SWRITE(debfd, msbuf);
						err++;
					}
			}
			if (!err) {
				strncpy(tagstr, ob->tag, 4);
				tagstr[4] = 0;
				sprintf(msbuf, "%5d%c%4s 0x%08lx[%4d]",
					ob->id,
					(ob->flg & OBALLOC ? '*' :
						(ob->flg & OBREALLOC ? '+' : ' ')),
					tagstr,
					(long) ob->base, ob->len);
				r = msbuf + strlen(msbuf);
				*r++ = ' ';
				if ((i = ob->len) > 24)
					i = 24;
				ascdump(&r, ob->base, i);
				(void)SWRITE(debfd, msbuf);
			}
			ob->flg &= ~(OBALLOC|OBREALLOC);
		}
	}
}
