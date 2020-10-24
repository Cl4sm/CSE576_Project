char *
localdomain()
{
	char *buf = 0;
	size_t buf_len = 0;
	int myerror = 0;

	do {
		errno = 0;

		if (buf) {
			buf_len += buf_len;
			if ((buf = realloc (buf, buf_len)) == NULL)
				err(1, NULL);
		} else {
			buf_len = 128;        /* Initial guess */
			if ((buf = malloc (buf_len)) == NULL)
				err(1, NULL);
		}
	} while (((myerror = getdomainname(buf, buf_len)) == 0 && !memchr (buf, '\0', buf_len))
	        || errno == ENAMETOOLONG);

	/* getdomainname failed, abort. */
	if (myerror)
		err(1, NULL);

	return buf;
}