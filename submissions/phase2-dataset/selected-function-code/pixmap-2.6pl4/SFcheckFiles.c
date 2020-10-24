SFcheckFiles(dir)
	SFDir	*dir;
{
	int		from, to;
	int		result;
	char		old, new;
	int		i;
	char		*str;
	int		last;
	struct stat	statBuf;

	result = 0;

	from = dir->vOrigin;
	to = dir->vOrigin + SFlistSize;
	if (to > dir->nEntries) {
		to = dir->nEntries;
	}

	for (i = from; i < to; i++) {
		str = dir->entries[i].real;
		last = strlen(str) - 1;
		old = str[last];
		str[last] = 0;
		if (stat(str, &statBuf)) {
			new = ' ';
		} else {
			new = SFstatChar(&statBuf);
		}
		str[last] = new;
		if (new != old) {
			result = 1;
		}
	}

	return result;
}
