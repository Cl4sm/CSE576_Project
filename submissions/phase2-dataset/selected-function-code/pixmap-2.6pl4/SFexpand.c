SFexpand(str)
	char	*str;
{
	int	len;
	int	cmp;
	char	*name, *growing;
	SFDir	*dir;
	SFEntry	*entry, *max;

	len = strlen(str);

	dir = &(SFdirs[SFdirEnd - 1]);

	if (dir->beginSelection == -1) {
		str = strcpy(XtMalloc((unsigned) (strlen(str) + 1)), str);
		SFreplaceText(dir, str);
		XtFree(str);
		return;
	} else if (dir->beginSelection == dir->endSelection) {
		SFreplaceText(dir, dir->entries[dir->beginSelection].shown);
		return;
	}

	max = &(dir->entries[dir->endSelection + 1]);

	name = dir->entries[dir->beginSelection].shown;
	(void) strcpy((growing = XtMalloc((unsigned) (strlen(name) + 1))),
		name);

	cmp = 0;
	while (!cmp) {
		entry = &(dir->entries[dir->beginSelection]);
		while (entry < max) {
			if (cmp = strncmp(growing, entry->shown, len)) {
				break;
			}
			entry++;
		}
		len++;
	}

	/*
	 * SFreplaceText() expects filename
	 */
	growing[len - 2] = ' ';

	growing[len - 1] = 0;
	SFreplaceText(dir, growing);
	XtFree(growing);
}
