set_value(vector strvec)
{
	char *str = VECTOR_SLOT(strvec, 1);
	size_t size;
	int i = 0;
	int len = 0;
	char *alloc = NULL;
	char *tmp;

	if (!str)
		return NULL;

	size = strlen(str);
	if (size == 0)
		return NULL;

	if (*str == '"') {
		for (i = 2; i < VECTOR_SIZE(strvec); i++) {
			str = VECTOR_SLOT(strvec, i);
			len += strlen(str);
			if (!alloc)
				alloc =
				    (char *) MALLOC(sizeof (char *) *
						    (len + 1));
			else {
				alloc =
				    REALLOC(alloc, sizeof (char *) * (len + 1));
				tmp = VECTOR_SLOT(strvec, i-1);
				if (alloc && *str != '"' && *tmp != '"')
					strncat(alloc, " ", 1);
			}

			if (alloc && i != VECTOR_SIZE(strvec)-1)
				strncat(alloc, str, strlen(str));
		}
	} else {
		alloc = MALLOC(sizeof (char *) * (size + 1));
		if (alloc)
			memcpy(alloc, str, size);
	}
	return alloc;
}
