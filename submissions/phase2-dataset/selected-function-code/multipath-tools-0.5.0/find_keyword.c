struct keyword *
find_keyword(vector v, char * name)
{
	struct keyword *keyword;
	int i;
	int len;

	if (!name || !keywords)
		return NULL;

	if (!v)
		v = keywords;

	len = strlen(name);

	for (i = 0; i < VECTOR_SIZE(v); i++) {
		keyword = VECTOR_SLOT(v, i);
		if ((strlen(keyword->string) == len) &&
		    !strcmp(keyword->string, name))
			return keyword;
		if (keyword->sub) {
			keyword = find_keyword(keyword->sub, name);
			if (keyword)
				return keyword;
		}
	}
	return NULL;
}
