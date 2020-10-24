SFgetHomeDirs()
{
	struct passwd	*pw;
	struct stat	statBuf;
	int		alloc;
	int		i;
	SFEntry		*entries = NULL;
#ifndef	HOME_ON_DEMAND /* by Y.Kawabe */
	int		len;
#endif	/* HOME_ON_DEMAND */
	int		maxChars;

	alloc = 0;
	i = 0;

	maxChars = -1;

	if (pw = getpwuid((int) getuid())) {
		if (
			(!stat(pw->pw_dir, &statBuf)) &&
			(S_ISDIR (statBuf.st_mode))
		) {
			alloc = 1;
			i = 1;
			entries = (SFEntry *) XtMalloc(sizeof(SFEntry));
			SFlogins = (SFLogin *) XtMalloc(sizeof(SFLogin));
			entries[0].real = XtMalloc(3);
			(void) strcpy(entries[0].real, "~");
			entries[0].shown = entries[0].real;
			entries[0].statDone = 1;
			SFlogins[0].name = "";
			SFlogins[0].dir = XtMalloc((unsigned)
				(strlen(pw->pw_dir) + 1));
			(void) strcpy(SFlogins[0].dir, pw->pw_dir);
			maxChars = 1;
		}
	}

#ifndef	HOME_ON_DEMAND /* by Y.Kawabe */

	(void) setpwent();

	while ((pw = getpwent()) && (*(pw->pw_name))) {
		if (
			(!stat(pw->pw_dir, &statBuf)) &&
			(S_ISDIR (statBuf.st_mode))
		) {
			if (i >= alloc) {
				alloc *= 2;
				entries = (SFEntry *) XtRealloc(
					(char *) entries,
					(unsigned) (alloc * sizeof(SFEntry))
				);
				SFlogins = (SFLogin *) XtRealloc(
					(char *) SFlogins,
					(unsigned) (alloc * sizeof(SFLogin))
				);
			}
			len = strlen(pw->pw_name);
			entries[i].real = XtMalloc((unsigned) (len + 3));
			(void) strcat(strcpy(entries[i].real, "~"),
				pw->pw_name);
			entries[i].shown = entries[i].real;
			entries[i].statDone = 1;
			if (len > maxChars) {
				maxChars = len;
			}
			SFlogins[i].name = XtMalloc((unsigned)
				(strlen(pw->pw_name) + 1));
			(void) strcpy(SFlogins[i].name, pw->pw_name);
			SFlogins[i].dir = XtMalloc((unsigned)
				(strlen(pw->pw_dir) + 1));
			(void) strcpy(SFlogins[i].dir, pw->pw_dir);
			i++;
		}
	}

#endif	/* HOME_ON_DEMAND */

	SFhomeDir.dir			= XtMalloc(1)	;
	SFhomeDir.dir[0]		= 0		;
	SFhomeDir.path			= SFcurrentPath	;
	SFhomeDir.entries		= entries	;
	SFhomeDir.nEntries		= i		;
	SFhomeDir.vOrigin		= 0		;	/* :-) */
	SFhomeDir.nChars		= maxChars + 2	;
	SFhomeDir.hOrigin		= 0		;
	SFhomeDir.changed		= 1		;
	SFhomeDir.beginSelection	= -1		;
	SFhomeDir.endSelection		= -1		;

#if defined(SVR4) || defined(SYSV) || defined(USG)
	qsort((char *) entries, (unsigned)i, sizeof(SFEntry), SFcompareEntries);
	qsort((char *) SFlogins, (unsigned)i, sizeof(SFLogin), SFcompareLogins);
#else /* defined(SVR4) || defined(SYSV) || defined(USG) */
	qsort((char *) entries, i, sizeof(SFEntry), SFcompareEntries);
	qsort((char *) SFlogins, i, sizeof(SFLogin), SFcompareLogins);
#endif /* defined(SVR4) || defined(SYSV) || defined(USG) */

	for (i--; i >= 0; i--) {
		(void) strcat(entries[i].real, "/");
	}
}
