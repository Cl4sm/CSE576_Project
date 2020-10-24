int
basenamecpy (const char * str1, char * str2, int str2len)
{
	char *p;

	if (!str1 || !strlen(str1))
		return 0;

	if (strlen(str1) >= str2len)
		return 0;

	if (!str2)
		return 0;

	p = (char *)str1 + (strlen(str1) - 1);

	while (*--p != '/' && p != str1)
		continue;

	if (p != str1)
		p++;

	strncpy(str2, p, str2len);
	str2[str2len - 1] = '\0';
	return strchop(str2);
}
