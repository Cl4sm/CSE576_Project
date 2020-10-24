{
	char *ret;

	if ((ret = setlocale (LC_CTYPE, "")) == NULL)
	{
		return (0);
	}

	if ((ret = nl_langinfo (CODESET)) == NULL)
	{
		return (0);
	}

	if (!strncasecmp (ret, "GB2312", 6))
	{
		return (GB_CODE);
	}
	if (!strncasecmp (ret, "GBK", 3))
	{
		return (GB_CODE);
	}
	else if (!strncasecmp (ret, "BIG5", 4))
	{
		return (BIG5_CODE);
	}
	else if (!strncasecmp (ret, "UTF8", 4))
	{
		return (UTF8_CODE);
	}
	else
		return (0);
}
