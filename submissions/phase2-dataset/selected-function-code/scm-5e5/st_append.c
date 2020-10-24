SCM st_append(args)
     SCM args;
{
	SCM res;
	register long i = 0;
	register SCM l, s;
	register unsigned char *data;
	for (l = args;NIMP(l);) {
		ASRTER(CONSP(l), l, ARGn, s_st_append);
		s = CAR(l);
		ASRTER(NIMP(s) && STRINGP(s), s, ARGn, s_st_append);
		i += LENGTH(s);
		l = CDR(l);
	}
	ASRTER(NULLP(l), args, ARGn, s_st_append);
	res = makstr(i);
	data = UCHARS(res);
	for (l = args;NIMP(l);l = CDR(l)) {
		s = CAR(l);
		for (i = 0;i<LENGTH(s);i++) *data++ = UCHARS(s)[i];
	}
	return res;
}
