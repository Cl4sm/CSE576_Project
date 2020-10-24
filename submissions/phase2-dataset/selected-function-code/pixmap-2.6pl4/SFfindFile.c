SFfindFile(dir, str)
	SFDir		*dir;
	register char	*str;
{
	register int	i, last, max;
	register char	*name, save;
	SFEntry		*entries;
	int		len;
	int		begin, end;
	int		result;

	len = strlen(str);

	if (str[len - 1] == ' ') {
		SFexpand(str);
		return 1;
	} else if (str[len - 1] == '/') {
		len--;
	}

	max = dir->nEntries;

	entries = dir->entries;

	i = 0;
	while (i < max) {
		name = entries[i].shown;
		last = strlen(name) - 1;
		save = name[last];
		name[last] = 0;

#ifdef SEL_FILE_IGNORE_CASE
		result = SFstrncmp(str, name, len);
#else /* def SEL_FILE_IGNORE_CASE */
		result = strncmp(str, name, len);
#endif /* def SEL_FILE_IGNORE_CASE */

		name[last] = save;
		if (result <= 0) {
			break;
		}
		i++;
	}
	begin = i;
	while (i < max) {
		name = entries[i].shown;
		last = strlen(name) - 1;
		save = name[last];
		name[last] = 0;

#ifdef SEL_FILE_IGNORE_CASE
		result = SFstrncmp(str, name, len);
#else /* def SEL_FILE_IGNORE_CASE */
		result = strncmp(str, name, len);
#endif /* def SEL_FILE_IGNORE_CASE */

		name[last] = save;
		if (result) {
			break;
		}
		i++;
	}
	end = i;

	if (begin != end) {
		if (
			(dir->beginSelection != begin) ||
			(dir->endSelection != end - 1)
		) {
			dir->changed = 1;
			dir->beginSelection = begin;
			if (str[strlen(str) - 1] == '/') {
				dir->endSelection = begin;
			} else {
				dir->endSelection = end - 1;
			}
		}
	} else {
		if (dir->beginSelection != -1) {
			dir->changed = 1;
			dir->beginSelection = -1;
			dir->endSelection = -1;
		}
	}

	if (
		SFdoNotTouchVorigin ||
		((begin > dir->vOrigin) && (end < dir->vOrigin + SFlistSize))
	) {
		SFdoNotTouchVorigin = 0;
		return 0;
	}

	i = begin - 1;
	if (i > max - SFlistSize) {
		i = max - SFlistSize;
	}
	if (i < 0) {
		i = 0;
	}

	if (dir->vOrigin != i) {
		dir->vOrigin = i;
		dir->changed = 1;
	}

	return 0;
}
