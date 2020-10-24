int
snprint_keyword(char *buff, int len, char *fmt, struct keyword *kw, void *data)
{
	int r;
	int fwd = 0;
	char *f = fmt;

	if (!kw || !kw->print)
		return 0;

	do {
		if (fwd == len || *f == '\0')
			break;
		if (*f != '%') {
			*(buff + fwd) = *f;
			fwd++;
			continue;
		}
		f++;
		switch(*f) {
		case 'k':
			fwd += snprintf(buff + fwd, len - fwd, "%s", kw->string);
			break;
		case 'v':
			r = kw->print(buff + fwd, len - fwd, data);
			if (!r) { /* no output if no value */
				buff = '\0';
				return 0;
			}
			fwd += r;
			break;
		}
		if (fwd > len)
			fwd = len;
	} while (*f++);
	return fwd;
}
