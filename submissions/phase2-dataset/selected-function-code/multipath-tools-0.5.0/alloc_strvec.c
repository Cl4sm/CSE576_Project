vector
alloc_strvec(char *string)
{
	char *cp, *start, *token;
	int strlen;
	int in_string;
	vector strvec;

	if (!string)
		return NULL;

	cp = string;

	/* Skip white spaces */
	while ((isspace((int) *cp) || !isascii((int) *cp)) && *cp != '\0')
		cp++;

	/* Return if there is only white spaces */
	if (*cp == '\0')
		return NULL;

	/* Return if string begin with a comment */
	if (*cp == '!' || *cp == '#')
		return NULL;

	/* Create a vector and alloc each command piece */
	strvec = vector_alloc();

	if (!strvec)
		return NULL;

	in_string = 0;
	while (1) {
		if (!vector_alloc_slot(strvec))
			goto out;

		start = cp;
		if (*cp == '"') {
			cp++;
			token = MALLOC(2);

			if (!token)
				goto out;

			*(token) = '"';
			*(token + 1) = '\0';
			if (in_string)
				in_string = 0;
			else
				in_string = 1;
		} else if (!in_string && (*cp == '{' || *cp == '}')) {
			token = MALLOC(2);

			if (!token)
				goto out;

			*(token) = *cp;
			*(token + 1) = '\0';
			cp++;
		} else {
			while ((in_string ||
				(!isspace((int) *cp) && isascii((int) *cp) &&
				 *cp != '!' && *cp != '#' && *cp != '{' &&
				 *cp != '}')) && *cp != '\0' && *cp != '"')
				cp++;
			strlen = cp - start;
			token = MALLOC(strlen + 1);

			if (!token)
				goto out;

			memcpy(token, start, strlen);
			*(token + strlen) = '\0';
		}
		vector_set_slot(strvec, token);

		while ((isspace((int) *cp) || !isascii((int) *cp))
		       && *cp != '\0')
			cp++;
		if (*cp == '\0' || *cp == '!' || *cp == '#')
			return strvec;
	}
out:
	vector_free(strvec);
	return NULL;
}
