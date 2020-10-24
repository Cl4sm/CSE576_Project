char *unistr_to_utf8(const unistr *str)
{
	unsigned int i;
	size_t len = 0;
	char *ret;
	char *p;

	for (i=0; i<str->len; i++) {
		if (str->chars[i] <= 0x7F) {
			len++;
		} else if (str->chars[i] <= 0x7FF) {
			len += 2;
		} else if (str->chars[i] <= 0xFFFF) {
			len += 3;
		} else if (str->chars[i] <= 0x1FFFFF) {
			len += 4;
		} else if (str->chars[i] <= 0x3FFFFFF) {
			len += 5;
		} else if (str->chars[i] <= 0x7FFFFFFF) {
			len += 6;
		} else {
			errno = 0;
			log_error(LOG_ARGS, "unistr_to_utf8(): can not utf-8 encode"
					"U+%04X", str->chars[i]);
			return mystrdup("");
		}
	}
	len++;  /* NUL */

	ret = mymalloc(len);
	p = ret;

	for (i=0; i<str->len; i++) {
		if (str->chars[i] <= 0x7F) {  /* 1 */
			*(p++) = str->chars[i];
		} else if (str->chars[i] <= 0x7FF) {  /* 2 */
			*(p++) = 192 + ((str->chars[i] & 1984) >> 6);
			*(p++) = 128 + (str->chars[i] & 63);
		} else if (str->chars[i] <= 0xFFFF) {  /* 3 */
			*(p++) = 224 + ((str->chars[i] & 61440) >> 12);
			*(p++) = 128 + ((str->chars[i] & 4032) >> 6);
			*(p++) = 128 + (str->chars[i] & 63);
		} else if (str->chars[i] <= 0x1FFFFF) {  /* 4 */
			*(p++) = 240 + ((str->chars[i] & 1835008) >> 18);
			*(p++) = 128 + ((str->chars[i] & 258048) >> 12);
			*(p++) = 128 + ((str->chars[i] & 4032) >> 6);
			*(p++) = 128 + (str->chars[i] & 63);
		} else if (str->chars[i] <= 0x3FFFFFF) {  /* 5 */
			*(p++) = 248 + ((str->chars[i] & 50331648) >> 24);
			*(p++) = 128 + ((str->chars[i] & 16515072) >> 18);
			*(p++) = 128 + ((str->chars[i] & 258048) >> 12);
			*(p++) = 128 + ((str->chars[i] & 4032) >> 6);
			*(p++) = 128 + (str->chars[i] & 63);
		} else if (str->chars[i] <= 0x7FFFFFFF) {  /* 6 */
			*(p++) = 252 + ((str->chars[i] & 1073741824) >> 30);
			*(p++) = 128 + ((str->chars[i] & 1056964608) >> 24);
			*(p++) = 128 + ((str->chars[i] & 16515072) >> 18);
			*(p++) = 128 + ((str->chars[i] & 258048) >> 12);
			*(p++) = 128 + ((str->chars[i] & 4032) >> 6);
			*(p++) = 128 + (str->chars[i] & 63);
		} else {
			errno = 0;
			log_error(LOG_ARGS, "unistr_to_utf8(): can not utf-8 encode"
					"U+%04X", str->chars[i]);
		}
	}
	*(p++) = '\0';

	return ret;
}