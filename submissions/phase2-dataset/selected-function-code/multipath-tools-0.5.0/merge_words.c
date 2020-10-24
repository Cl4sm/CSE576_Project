static int
merge_words (char ** dst, char * word, int space)
{
	char * p;
	int len;

	len = strlen(*dst) + strlen(word) + space;
	*dst = REALLOC(*dst, len + 1);

	if (!*dst)
		return 1;

	p = *dst;

	while (*p != '\0')
		p++;

	while (space) {
		*p = ' ';
		p++;
		space--;
	}
	strncpy(p, word, strlen(word) + 1);

	return 0;
}
