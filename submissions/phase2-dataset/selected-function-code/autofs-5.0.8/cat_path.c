int cat_path(char *buf, size_t len, const char *dir, const char *base)
{
	char *d = (char *) dir;
	char *b = (char *) base;
	char *s = buf;
	size_t left = len;

	if ((*s = *d))
		while ((*++s = *++d) && --left) ;
	
	if (!left) {
		*s = '\0';
		return 0;
	}

	/* Now we have at least 1 left in output buffer */

	while (*--s == '/' && (left++ < len))
		*s = '\0';

	*++s = '/';
	left--;

	if (*b == '/') 
		while (*++b == '/');

	while (--left && (*++s = *b++)) ;

	if (!left) {
		*s = '\0';
		return 0;
	}

	return 1;
}