pvmstrtoi(p)
	char *p;
{
	int i = 0;
	char c;

	if (*p == '0') {
		p++;
		if (*p == 'x' || *p == 'X')
			while (isxdigit(c = *++p))
				i = (i << 4) + c
						- (isdigit(c) ? '0' : (isupper(c) ? 'A' : 'a') - 10);
		else
			while (c = *p++, c >= '0' && c <= '7')
				i = (i << 3) + c - '0';
		return i;
	} else
		return (int)atoi(p);
}
