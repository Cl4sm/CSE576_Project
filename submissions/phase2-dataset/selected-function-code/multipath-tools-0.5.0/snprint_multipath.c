snprint_multipath (char * line, int len, char * format,
	     struct multipath * mpp)
{
	char * c = line;   /* line cursor */
	char * s = line;   /* for padding */
	char * f = format; /* format string cursor */
	int fwd;
	struct multipath_data * data;
	char buff[MAX_FIELD_LEN] = {};

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

		if (!(data = mpd_lookup(*f)))
			continue;

		data->snprint(buff, MAX_FIELD_LEN, mpp);
		PRINT(c, TAIL, "%s", buff);
		PAD(data->width);
		buff[0] = '\0';
	} while (*f++);

	ENDLINE;
	return (c - line);
}
