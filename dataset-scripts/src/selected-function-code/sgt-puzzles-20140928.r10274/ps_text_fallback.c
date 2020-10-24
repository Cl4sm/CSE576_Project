static char *ps_text_fallback(void *handle, const char *const *strings,
			      int nstrings)
{
    /*
     * We can handle anything in ISO 8859-1, and we'll manually
     * translate it out of UTF-8 for the purpose.
     */
    int i, maxlen;
    char *ret;

    maxlen = 0;
    for (i = 0; i < nstrings; i++) {
	int len = strlen(strings[i]);
	if (maxlen < len) maxlen = len;
    }

    ret = snewn(maxlen + 1, char);

    for (i = 0; i < nstrings; i++) {
	const char *p = strings[i];
	char *q = ret;

	while (*p) {
	    int c = (unsigned char)*p++;
	    if (c < 0x80) {
		*q++ = c;	       /* ASCII */
	    } else if ((c == 0xC2 || c == 0xC3) && (*p & 0xC0) == 0x80) {
		*q++ = (c << 6) | (*p++ & 0x3F);   /* top half of 8859-1 */
	    } else {
		break;
	    }
	}

	if (!*p) {
	    *q = '\0';
	    return ret;
	}
    }

    assert(!"Should never reach here");
    return NULL;
}