char *unistr_utf8_to_header(const char *str)
{
	unistr *us;
	char *ret;
	const char *p;
	int clean;
	char buf[4];

	/* clean header? */
	clean = 1;
	for (p=str; *p; p++) {
		if (!is_ok_in_header(*p)) {
			clean = 0;
			break;
		}
	}
	if (clean) {
		return mystrdup(str);
	}

	us = unistr_new();

	unistr_append_usascii(us, "=?utf-8?q?", 10);
	for (p=str; *p; p++) {
		if (*p == 0x20) {
			unistr_append_char(us, '_');
		} else if (is_ok_in_header(*p)) {
			unistr_append_char(us, *p);
		} else {
			snprintf(buf, sizeof(buf), "=%02X", (unsigned char)*p);
			unistr_append_usascii(us, buf, 3);
		}
	}
	unistr_append_usascii(us, "?=", 2);

	ret = unistr_to_utf8(us);
	unistr_free(us);

	return ret;
}