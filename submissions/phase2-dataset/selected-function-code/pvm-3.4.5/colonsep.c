colonsep(s)
	char *s;	/* the string to break up */
{
	char **els;
	int nel = 2;			/* length of els */
	char *p, *q;

#if defined (IMA_OS2) || defined (WIN32)
	for (p = s; p = CINDEX(p, ';'); p++)
#else
	for (p = s; p = CINDEX(p, ':'); p++)
#endif
		nel++;
	els = TALLOC(nel, char*, "path");

	nel = 0;
	for (p = s; p; p = q) {
#if defined (IMA_OS2) || defined (WIN32)
		if (q = CINDEX(p, ';'))
#else
		if (q = CINDEX(p, ':'))
#endif
			*q++ = 0;
		els[nel++] = p;
	}
	els[nel] = 0;
	return els;
}
