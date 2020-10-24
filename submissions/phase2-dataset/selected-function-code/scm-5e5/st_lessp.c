     SCM s1, s2;
{
	register sizet i, len;
	register unsigned char *c1, *c2;
	register int c;
	ASRTER(NIMP(s1) && STRINGP(s1), s1, ARG1, s_st_lessp);
	ASRTER(NIMP(s2) && STRINGP(s2), s2, ARG2, s_st_lessp);
	len = LENGTH(s1);
	i = LENGTH(s2);
	if (len>i) i = len;
	c1 = UCHARS(s1);
	c2 = UCHARS(s2);
	for (i = 0;i<len;i++) {
		c = (*c1++ - *c2++);
		if (c>0) return BOOL_F;
		if (c<0) return BOOL_T;
	}
	return (LENGTH(s2) != len) ? BOOL_T : BOOL_F;
}
