int
dm_taskout(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	int tid;
	int l;
	char *p, *q, c;
	char buf2[32];
	char buf[4100];		 /* XXX a bit bigger than in pvmd.c`loclstout() */

	hp = hp;

	if (upkuint(mp, &tid) || upkint(mp, &l) || l < 1)
		return 0;

	/* unpack data, leaving room at head of buffer */

	p = buf + 32;
	if (l > sizeof(buf) - (p - buf) - 2)
		l = sizeof(buf) - (p - buf) - 2;
	upkbyte(mp, p, l);

	/* ends with "\n\0" */
	if (p[l - 1] != '\n')
		p[l++] = '\n';
	p[l] = 0;

	sprintf(buf2, "[t%x] ", tid);
	l = strlen(buf2);
	while (*p) {
		for (q = p; *q++ != '\n'; ) ;
		c = *q;
		*q = 0;
		BCOPY(buf2, p - l, l);
		pvmlogerror(p - l);
		*q = c;
		p = q;
	}
	return 0;
}
