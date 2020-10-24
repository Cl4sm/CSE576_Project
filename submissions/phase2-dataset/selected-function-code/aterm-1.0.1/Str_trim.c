char           *
Str_trim(char *str)
{
    if (str && *str) {
	char           *src;
	int             n;

	src = Str_skip_space(str);
	n = strlen(src) - 1;

	while (n > 0 && isspace(src[n]))
	    n--;
	src[n + 1] = '\0';

    /* strip leading/trailing quotes */
	if (src[0] == '"') {
	    src++;
	    n--;
	    if (src[n] == '"')
		src[n--] = '\0';
	}
	if (n < 0)
	    *str = '\0';
	else if (src != str) {
	    char           *dst = str;

	/* copy back in-place */
	    do {
		*dst++ = *src;
	    }
	    while (*src++ != '\0');
	}
    }
    return str;
}
