char *substitute(const char *line, const char *listaddr, const char *listdelim,
		const char *listdir, text *txt)
{
	char *new;
	char *pos;
	int width = 0; /* Just a dummy here */

	new = mystrdup(line);
	pos = new;

	while (*pos != '\0') {
		if (*pos == '$') {
			substitute_one(&new, &pos, &width,
					listaddr, listdelim, listdir, txt);
			/* The function sets up for the next character
			 * to process, so continue straight away. */
			continue;
		}
		pos++;
	}

	return new;
}