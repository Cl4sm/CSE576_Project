static int get_percent_decoded_len(const char *name)
{
	int escapes = 0;
	int escaped = 0;
	const char *tmp = name;
	int look_for_close = 0;

	while (*tmp) {
		if (*tmp == '%') {
			/* assume escapes aren't interpreted inside brackets */
			if (look_for_close) {
				tmp++;
				continue;
			}
			/* check for escaped % */
			if (escaped) {
				tmp++;
				escaped = 0;
				continue;
			}
			escapes++;
			tmp++;
			if (*tmp == '[') {
				escapes++;
				tmp++;
				look_for_close = 1;
			} else
				escaped = 1;
		} else if (*tmp == ']' && look_for_close) {
			escaped = 0;
			escapes++;
			tmp++;
			look_for_close = 0;
		} else {
			tmp++;
			escaped = 0;
		}
	}

	assert(strlen(name) > escapes);
	return strlen(name) - escapes;
}