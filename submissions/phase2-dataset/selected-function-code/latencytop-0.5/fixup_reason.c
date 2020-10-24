{
	char *c2;

	c2 = strchr(c, '\n');
	if (c2)
		*c2=0;
	while (c[0]==' ')
		c++;
	strncpy(line->backtrace, c, 4096);
	c2 = translate(c);
	if (c2 == NULL) {
		line->reason[0] = '[';
		strncpy(line->reason + 1, c, 1022);
		for (c2 = line->reason + 1; *c2 && (c2 - line->reason) < 1022; c2++)
			if (*c2 == ' ')
				break;
		*(c2++) = ']';
		*(c2++) = 0;
	} else
		strncpy(line->reason, c2, 1024);
}
