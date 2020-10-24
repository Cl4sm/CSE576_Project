snprint_progress (char * buff, size_t len, int cur, int total)
{
	char * c = buff;
	char * end = buff + len;

	if (total > 0) {
		int i = PROGRESS_LEN * cur / total;
		int j = PROGRESS_LEN - i;

		while (i-- > 0) {
			c += snprintf(c, len, "X");
			if ((len = (end - c)) <= 1) goto out;
		}

		while (j-- > 0) {
			c += snprintf(c, len,  ".");
			if ((len = (end - c)) <= 1) goto out;
		}
	}

	c += snprintf(c, len, " %i/%i", cur, total);

out:
	buff[c - buff + 1] = '\0';
	return (c - buff + 1);
}
