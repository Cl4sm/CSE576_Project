SFgetDir(dir)
	SFDir	*dir;
{
	SFEntry		*result = NULL;
	int		alloc = 0;
	int		i;
	DIR		*dirp;
	struct dirent	*dp;
	char		*str;
	int		len;
	int		maxChars;
	struct stat	statBuf;

	maxChars = strlen(dir->dir) - 1;

	dir->entries = NULL;
	dir->nEntries = 0;
	dir->nChars = 0;

	result = NULL;
	i = 0;

	dirp = opendir(".");
	if (!dirp) {
		return 1;
	}

	(void) stat(".", &statBuf);
	dir->mtime = statBuf.st_mtime;

	(void) readdir(dirp);	/* throw away "." */

#ifndef S_IFLNK
	(void) readdir(dirp);	/* throw away ".." */
#endif /* ndef S_IFLNK */

	while (dp = readdir(dirp)) {
		if (i >= alloc) {
			alloc = 2 * (alloc + 1);
			result = (SFEntry *) XtRealloc((char *) result,
				(unsigned) (alloc * sizeof(SFEntry)));
		}
		result[i].statDone = 0;
		str = dp->d_name;
		len = strlen(str);
		result[i].real = XtMalloc((unsigned) (len + 2));
		(void) strcat(strcpy(result[i].real, str), " ");
		if (len > maxChars) {
			maxChars = len;
		}
		result[i].shown = result[i].real;
		i++;
	}

#if defined(SVR4) || defined(SYSV) || defined(USG)
	qsort((char *) result, (unsigned) i, sizeof(SFEntry), SFcompareEntries);
#else /* defined(SVR4) || defined(SYSV) || defined(USG) */
	qsort((char *) result, i, sizeof(SFEntry), SFcompareEntries);
#endif /* defined(SVR4) || defined(SYSV) || defined(USG) */

	dir->entries = result;
	dir->nEntries = i;
	dir->nChars = maxChars + 1;

	closedir(dirp);

	return 0;
}
