char *
localhost (void)
{
	static char *buf = 0;
	static size_t buf_len = 0;
	int myerror = 0;

	if (! buf) {
		do {
			errno = 0;
			if (buf) {
				buf_len += buf_len;
				buf = realloc (buf, buf_len);
			} else {
				buf_len = 128;        /* Initial guess */
				buf = malloc (buf_len);
			}
			if (! buf) {
				errno = ENOMEM;
				return 0;
			}
		} while ( (
			 	(myerror = gethostname(buf, buf_len)) == 0
					&& !memchr (buf, '\0', buf_len))
				|| errno == ENAMETOOLONG
			);

		if (myerror) {
			/* gethostname failed, abort.  */
			free (buf);
			buf = "(unknown host)";
		}
	}
	return buf;
}