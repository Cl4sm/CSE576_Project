static struct oc_text_buf *get_qs(char **str)
{
	struct oc_text_buf *res;
	int escaped = 0;
	char *p = *str;

	if (*p != '\"')
		return NULL;

	res = buf_alloc();

	while (*++p) {
		if (!escaped && *p == '\"') {
			*str = p+1;
			if (buf_error(res))
				break;
			return res;
		}
		if (escaped)
			escaped = 0;
		else if (*p == '\\')
			escaped = 1;
		buf_append_bytes(res, p, 1);
	}
	buf_free(res);
	return NULL;
}