static int decode_percent_hack(const char *name, char **key)
{
	const char *tmp;
	char *ptr, *new;
	unsigned int len;
	int escaped = 0, look_for_close = 0;

	if (!key)
		return -1;

	*key = NULL;

	len = get_percent_decoded_len(name);
	new = malloc(len + 1);
	if (!new)
		return -1;

	ptr = new;
	tmp = name;
	while (*tmp) {
		if (*tmp == '%') {
			if (escaped) {
				*ptr++ = *tmp++;
				if (!look_for_close)
					escaped = 0;
				continue;
			}
			tmp++;
			if (*tmp == '[') {
				tmp++;
				look_for_close = 1;
				escaped = 1;
			} else
				escaped = 1;
		} else if (*tmp == ']' && look_for_close) {
			tmp++;
			look_for_close = 0;
		} else {
			escaped = 0;
			*ptr++ = *tmp++;
		}
	}
	*ptr = '\0';
	*key = new;

	validate_string_len(name, new, ptr, len);
	return strlen(new);
}