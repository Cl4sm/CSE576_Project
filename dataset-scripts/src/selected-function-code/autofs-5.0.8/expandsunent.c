int expandsunent(const char *src, char *dst, const char *key,
		 const struct substvar *svc, int slashify_colons)
{
	const struct substvar *sv;
	int len, l, seen_colons;
	const char *p;
	char ch;

	len = 0;
	seen_colons = 0;

	while ((ch = *src++)) {
		switch (ch) {
		case '&':
			l = strlen(key);
			/*
			 * In order to ensure that any isspace() characters
			 * in the key are preserved, we need to escape them
			 * here.
			 */
			const char *keyp = key;
			while (*keyp) {
				if (isspace(*keyp)) {
					if (dst) {
						*dst++ = '\\';
						*dst++ = *keyp++;
					} else
						keyp++;
					l++;
				} else {
					if (dst)
						*dst++ = *keyp++;
					else
						keyp++;
				}
			}
			len += l;
			break;

		case '$':
			if (*src == '{') {
				p = strchr(++src, '}');
				if (!p) {
					/* Ignore rest of string */
					if (dst)
						*dst = '\0';
					return len;
				}
				sv = macro_findvar(svc, src, p - src);
				if (sv) {
					l = strlen(sv->val);
					if (dst) {
						strcpy(dst, sv->val);
						dst += l;
					}
					len += l;
				}
				src = p + 1;
			} else {
				p = src;
				while (isalnum(*p) || *p == '_')
					p++;
				sv = macro_findvar(svc, src, p - src);
				if (sv) {
					l = strlen(sv->val);
					if (dst) {
						strcpy(dst, sv->val);
						dst += l;
					}
					len += l;
				}
				src = p;
			}
			break;

		case '\\':
			len++;
			if (dst)
				*dst++ = ch;

			if (*src) {
				len++;
				if (dst)
					*dst++ = *src;
				src++;
			}
			break;

		case '"':
			len++;
			if (dst)
				*dst++ = ch;

			while (*src && *src != '"') {
				len++;
				if (dst)
					*dst++ = *src;
				src++;
			}
			if (*src && dst) {
				len++;
				*dst++ = *src++;
			}
			break;

		case ':':
			if (dst)
				*(dst++) = 
				  (seen_colons && slashify_colons) ? '/' : ':';
			len++;
			/* Were looking for the colon preceeding a path */
			if (*src == '/')
				seen_colons = 1;
			break;

		default:
			if (isspace(ch))
				seen_colons = 0;

			if (dst)
				*(dst++) = ch;
			len++;
			break;
		}
	}
	if (dst)
		*dst = '\0';
	return len;
}