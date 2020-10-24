snprint_multipath_header (char * line, int len, char * format)
{
	char * c = line;   /* line cursor */
	char * s = line;   /* for padding */
	char * f = format; /* format string cursor */
	int fwd;
	struct multipath_data * data;

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
			continue; /* unknown wildcard */

		PRINT(c, TAIL, "%s", data->header);
		PAD(data->width);
	} while (*f++);

	ENDLINE;
	return (c - line);
}
