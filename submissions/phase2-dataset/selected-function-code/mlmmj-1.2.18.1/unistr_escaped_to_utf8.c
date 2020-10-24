char *unistr_escaped_to_utf8(const char *str)
{
	unistr_char ch;
	unistr *us;
	char *ret;
	char u[5];
	int len;
	int skip = 0;

	us = unistr_new();

	while (*str) {
		if (*str == '\\') {
			str++;
			if (*str == 'u' && !skip) {
				str++;
				if (!isxdigit(str[0]) ||
						!isxdigit(str[1]) ||
						!isxdigit(str[2]) ||
						!isxdigit(str[3])) {
					unistr_append_char(us, '?');
					continue;
				}
				u[0] = *str++;
				u[1] = *str++;
				u[2] = *str++;
				u[3] = *str++;
				u[4] = '\0';
				ch = strtol(u, NULL, 16);
				unistr_append_char(us, ch);
				continue;
			} else {
				unistr_append_char(us, '\\');
				/* Avoid processing the second backslash of a
				 * double-backslash; but if this was a such a
				 * one, go back to normal */
				skip = !skip;
				continue;
			}
		} else {
			u[0] = *str;
			len = 1;
			str++;
			while (*str && (unsigned char)u[0] > 0x7F) {
				u[0] = *str;
				len++;
				str++;
			}
			unistr_append_utf8(us, str - len, len);
		}
	}

	ret = unistr_to_utf8(us);
	unistr_free(us);

	return ret;
}