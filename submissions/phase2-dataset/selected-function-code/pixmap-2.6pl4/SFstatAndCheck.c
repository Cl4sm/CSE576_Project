SFstatAndCheck(dir, entry)
	SFDir	*dir;
	SFEntry	*entry;
{
	struct stat	statBuf;
	char		save;
	int		last;

	/*
	 * must be restored before returning
	 */
	save = *(dir->path);
	*(dir->path) = 0;

	if (!SFchdir(SFcurrentPath)) {
		last = strlen(entry->real) - 1;
		entry->real[last] = 0;
		entry->statDone = 1;
		if (
			(!stat(entry->real, &statBuf))

#ifdef S_IFLNK

		     || (!lstat(entry->real, &statBuf))

#endif /* ndef S_IFLNK */

		) {
			if (SFfunc) {
				char *shown;

				shown = NULL;
				if (SFfunc(entry->real, &shown, &statBuf)) {
					if (shown) {
						int len;

						len = strlen(shown);
						entry->shown = XtMalloc(
							(unsigned) (len + 2)
						);
						(void) strcpy(entry->shown,
							shown);
						SFwriteStatChar(
							entry->shown,
							len,
							&statBuf
						);
						entry->shown[len + 1] = 0;
					}
				} else {
					SFdeleteEntry(dir, entry);

					*(dir->path) = save;
					return 1;
				}
			}
			SFwriteStatChar(entry->real, last, &statBuf);
		} else {
			entry->real[last] = ' ';
		}
	}

	*(dir->path) = save;
	return 0;
}
