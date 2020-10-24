int
_install_keyword(char *string, int (*handler) (vector),
		int (*print) (char *, int, void *), int unique)
{
	int i = 0;
	struct keyword *keyword;

	/* fetch last keyword */
	keyword = VECTOR_SLOT(keywords, VECTOR_SIZE(keywords) - 1);

	/* position to last sub level */
	for (i = 0; i < sublevel; i++)
		keyword =
		    VECTOR_SLOT(keyword->sub, VECTOR_SIZE(keyword->sub) - 1);

	/* First sub level allocation */
	if (!keyword->sub)
		keyword->sub = vector_alloc();

	if (!keyword->sub)
		return 1;

	/* add new sub keyword */
	return keyword_alloc(keyword->sub, string, handler, print, unique);
}
