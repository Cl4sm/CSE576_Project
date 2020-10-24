{

	if (!strncasecmp (input, "GB", 2))
	{
		*encode = GB_CODE;
		_initgb ();
	}
	else if (!strncasecmp (input, "BIG5", 4))
	{
		*encode = BIG5_CODE;
		_initbig ();
	}
	else if (!strncasecmp (input, "UTF8", 4))
	{
		*encode = UTF8_CODE;
		_initutf8 ();
	}
	else
	{
		printf ("encoding %s is not supported.\n", input);
		printusage ();
	}
}
