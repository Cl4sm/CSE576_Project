char *unistr_header_to_utf8(const char *str)
{
	char *my_str;
	char *word;
	char *p;
	unistr *us;
	char *ret;

	my_str = mystrdup(str);
	us = unistr_new();

	word = strtok_r(my_str, " \t\n", &p);
	while (word) {
		header_decode_word(word, us);
		word = strtok_r(NULL, " \t\n", &p);
		if (word)
			unistr_append_char(us, ' ');
	}

	myfree(my_str);

	ret = unistr_to_utf8(us);
	unistr_free(us);

	return ret;
}