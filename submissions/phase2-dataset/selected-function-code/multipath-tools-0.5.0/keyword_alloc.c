int
keyword_alloc(vector keywords, char *string, int (*handler) (vector),
		int (*print) (char *, int, void *), int unique)
{
	struct keyword *keyword;

	keyword = (struct keyword *) MALLOC(sizeof (struct keyword));

	if (!keyword)
		return 1;

	if (!vector_alloc_slot(keywords)) {
		FREE(keyword);
		return 1;
	}
	keyword->string = string;
	keyword->handler = handler;
	keyword->print = print;
	keyword->unique = unique;

	vector_set_slot(keywords, keyword);

	return 0;
}
