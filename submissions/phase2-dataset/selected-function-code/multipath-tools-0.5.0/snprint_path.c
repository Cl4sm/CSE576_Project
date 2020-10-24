int
snprint_path (char * line, int len, char * format,
	     struct path * pp)
{
	char * c = line;   /* line cursor */
	char * s = line;   /* for padding */
	char * f = format; /* format string cursor */
	int fwd;
	struct path_data * data;
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

		if (!(data = pd_lookup(*f)))
			continue;

		data->snprint(buff, MAX_FIELD_LEN, pp);
		PRINT(c, TAIL, "%s", buff);
		PAD(data->width);
	} while (*f++);

	ENDLINE;
	return (c - line);
}
