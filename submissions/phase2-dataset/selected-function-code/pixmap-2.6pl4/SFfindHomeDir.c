static int
SFfindHomeDir(begin, end)
	char	*begin, *end;
{
	char	save;
	char	*theRest;
	int	i;

	save = *end;
	*end = 0;

	for (i = SFhomeDir.nEntries - 1; i >= 0; i--) {
		if (!strcmp(SFhomeDir.entries[i].real, begin)) {
			*end = save;
			theRest = XtMalloc((unsigned) (strlen(end) + 1));
			(void) strcpy(theRest, end);
			(void) strcat(strcat(strcpy(SFcurrentPath,
				SFlogins[i].dir), "/"), theRest);
			XtFree(theRest);
			SFsetText(SFcurrentPath);
			SFtextChanged();
			return 1;
		}
	}

#ifdef	HOME_ON_DEMAND /* by Y.Kawabe */
	if (*(++begin)) {
		struct passwd *pwd;
		char user[256];
		int i = 0;

		while (*begin && *begin != '/' && i < sizeof(user))
			user[i++] = *begin++;
		user[i] = '\0';

		if (pwd = getpwnam(user)) {
			theRest = XtMalloc((unsigned) (strlen(end) + 1));
			(void) strcpy(theRest, end);
			(void) strcat(strcat(strcpy(SFcurrentPath,
				pwd->pw_dir), "/"), theRest);
			XtFree(theRest);
			SFsetText(SFcurrentPath);
			SFtextChanged();
			return 1;
		}
	}
#endif	/* HOME_ON_DEMAND */

	*end = save;

	return 0;
}
