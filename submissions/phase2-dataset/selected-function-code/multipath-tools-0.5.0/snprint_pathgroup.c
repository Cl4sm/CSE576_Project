int
snprint_pathgroup (char * line, int len, char * format,
		   struct pathgroup * pgp)
{
	char * c = line;   /* line cursor */
	char * s = line;   /* for padding */
	char * f = format; /* format string cursor */
	int fwd;
	struct pathgroup_data * data;
	char buff[MAX_FIELD_LEN];

	memset(line, 0, len);

	do {
		if (!TAIL)
			break;

		if (*f != '%') {
			*c++ = *f;
			NOPAD;
			continue;
		}
		f++;

		if (!(data = pgd_lookup(*f)))
			continue;

		data->snprint(buff, MAX_FIELD_LEN, pgp);
		PRINT(c, TAIL, "%s", buff);
		PAD(data->width);
	} while (*f++);

	ENDLINE;
	return (c - line);
}
