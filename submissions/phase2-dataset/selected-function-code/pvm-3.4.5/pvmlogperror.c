int
pvmlogperror(s)
	char *s;	/* text */
{
#ifdef WIN32
	char em[16];
	int error_nr=0;
#else
#ifdef SYSERRISCONST
	const char *em;
#else
	char *em;
#endif
#endif
	int i;

#ifndef WIN32
#ifdef USESTRERROR
	em = (char *) strerror( errno );
#else
	em = ((errno >= 0 && errno < sys_nerr)
		? sys_errlist[errno] : "Unknown Error");
#endif
#else
	error_nr = GetLastError();
	sprintf(em,"%d",error_nr);
#endif

	pvmlogprintf("%s: ", s);
	pvmlogprintf("errno=%d", errno);
	pvmlogprintf("\n");

	pvmlogprintf("em=0x%lx\n", (long) em);

	for (i=0; i < 10 ; i++ )
		pvmlogprintf("[%x/%c]", em[i]);
	pvmlogprintf("\n");

	pvmlogprintf("%s: %s\n", s, em);
	return 0;
}
