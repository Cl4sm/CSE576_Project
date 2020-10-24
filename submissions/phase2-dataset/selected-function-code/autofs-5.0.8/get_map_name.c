static char *get_map_name(const char *string)
{
	char *name, *tmp;
	char *start, *end, *base;

	tmp = strdup(string);
	if (!tmp) {
		printf("error: allocation failure: %s\n", strerror(errno));
		return NULL;
	}

	base = basename(tmp);
	end = strchr(base, ',');
	if (end)
		*end = '\0';
	start = strchr(tmp, '=');
	if (start)
		start++;
	else {
		char *colon = strrchr(base, ':');
		if (colon)
			start = ++colon;
		else
			start = base;
	}

	name = strdup(start);
	if (!name)
		printf("error: allocation failure: %s\n", strerror(errno));
	free(tmp);

	return name;
}