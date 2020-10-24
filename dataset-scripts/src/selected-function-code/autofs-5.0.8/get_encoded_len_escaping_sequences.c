static int get_encoded_len_escaping_sequences(const char *name)
{
	const char *tmp;
	unsigned int escapes = 0;

	tmp = name;
	while (*tmp) {
		/* escape percents */
		if (*tmp == '%')
			escapes++;
		else if (isupper(*tmp)) {
			/* start an escape block %[...] */
			escapes += 3;  /* %[] */
			while (*tmp && isupper(*tmp))
				tmp++;
			continue;
		}
		tmp++;
	}

	return strlen(name) + escapes;
}