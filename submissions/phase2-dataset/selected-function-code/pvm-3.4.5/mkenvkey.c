static
char *mkenvkey(char mch)
{
#ifndef __EMX__
	static char skey[40];
	register struct utmp *putmp; 
	char stemp[40], stemp1[40], sttyname[30];
	register char *sk,*p;

	if (isatty(2))
		strcpy(sttyname,ttyname(2));
	else if (isatty(0))
		strcpy(sttyname,ttyname(0));
	else if (isatty(1))
		strcpy(sttyname,ttyname(1));
	else
#endif
		return (NULL);
#ifndef __EMX__
	putmp = getutmp(sttyname);
	if (putmp == NULL)
		return (NULL);
	sk = skey;
	p = putmp->ut_line;
	while (*p)
		*sk++ = *p++;
	*sk++ = mch;
	(void)sprintf(stemp, "%ld", putmp->ut_time);
	sreverse(stemp1, stemp);
	p = stemp1;
	while (*p)
		*sk++ = *p++;
	*sk = 0;
	return (skey);
#endif
}
