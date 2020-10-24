enum subtype is_subbed(const char *listdir, const char *address, int both)
{
	int retval;
	char *subddirname;
	enum subtype typesub = SUB_NONE;
	
	subddirname = concatstr(2, listdir, "/subscribers.d/");
	retval = is_subbed_in(subddirname, address);
	myfree(subddirname);
	if (retval) {
		if (!both) return SUB_NORMAL;
		typesub = SUB_NORMAL;
	}

	subddirname = concatstr(2, listdir, "/digesters.d/");
	retval = is_subbed_in(subddirname, address);
	myfree(subddirname);
	if (retval) {
		if (typesub == SUB_NORMAL) return SUB_BOTH;
		return SUB_DIGEST;
	}

	subddirname = concatstr(2, listdir, "/nomailsubs.d/");
	retval = is_subbed_in(subddirname, address);
	myfree(subddirname);
	if (retval) return SUB_NOMAIL;

	return typesub;
}