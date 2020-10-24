static char *recipient_extra(const char *listdir, const char *addr)
{
	char *listdelim;
	char *delim, *atsign, *ret;
	size_t len;

	if (!addr)
		return NULL;

	listdelim = getlistdelim(listdir);

	delim = strstr(addr, listdelim);
	if (!delim) {
		myfree(listdelim);
		return NULL;
	}
	delim += strlen(listdelim);
	myfree(listdelim);

	atsign = strrchr(delim, '@');
	if (!atsign)
		return NULL;

	len = atsign - delim;
	ret = (char *)mymalloc(len + 1);
	strncpy(ret, delim, len);
	ret[len] = '\0';

	return ret;
}