static void encode_sequence(const char *name, char *new, unsigned int len)
{
	const char *tmp;
	char *ptr;

	ptr = new;
	tmp = name;
	while (*tmp) {
		if (*tmp == '%') {
			*ptr++ = '%';
			*ptr++ = *tmp++;
		} else if (isupper(*tmp)) {
			*ptr++ = '%';
			*ptr++ = '[';
			*ptr++ = *tmp++;

			while (*tmp && isupper(*tmp)) {
				*ptr++ = *tmp;
				tmp++;
			}
			*ptr++ = ']';
		} else
			*ptr++ = *tmp++;
	}
	*ptr = '\0';
	validate_string_len(name, new, ptr, len);
}