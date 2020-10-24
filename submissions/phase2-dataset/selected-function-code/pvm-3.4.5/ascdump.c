ascdump(o, p, e)
	char **o;		/* addr of pointer to output space (passed back) */
	char *p;		/* string to convert */
	int e;			/* length of string */
{
	char *r = *o;
	char c;

	while (e-- > 0) {
		c = 0x7f & *p;
		if (c < ' ' || c == 0x7f) {
			c = (c + '@') & 0x7f;
			*r++ = '^';
		} else
			*r++ = ' ';
		*r++ = c;
		p++;
	}
	*r++ = '\n';
	*r = 0;
	*o = r;
}
