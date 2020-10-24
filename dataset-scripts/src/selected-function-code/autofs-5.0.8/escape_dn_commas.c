static char *escape_dn_commas(const char *uri)
{
	size_t len = strlen(uri);
	char *new, *tmp, *ptr;

	ptr = (char *) uri;
	while (*ptr) {
		if (*ptr == '\\')
			ptr += 2;
		if (*ptr == ',')
			len += 2;
		ptr++;
	}

	new = malloc(len + 1);
	if (!new)
		return NULL;
	memset(new, 0, len + 1);

	ptr = (char *) uri;
	tmp = new;
	while (*ptr) {
		if (*ptr == '\\') {
			ptr++;
			*tmp++ = *ptr++;
			continue;
		}
		if (*ptr == ',') {
			strcpy(tmp, "%2c");
			ptr++;
			tmp += 3;
			continue;
		}
		*tmp++ = *ptr++;
	}

	return new;
}