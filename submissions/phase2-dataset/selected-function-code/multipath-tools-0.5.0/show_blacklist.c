int
show_blacklist (char ** r, int * len)
{
	char *c = NULL;
	char *reply = NULL;
	unsigned int maxlen = INITIAL_REPLY_LEN;
	int again = 1;

	reply = MALLOC(maxlen);

	while (again) {
		if (!reply)
			return 1;

		c = reply;
		c += snprint_blacklist_report(c, maxlen);
		again = ((c - reply) == maxlen);
		REALLOC_REPLY(reply, again, maxlen);
	}

	*r = reply;
	*len = (int)(c - reply + 1);

	return 0;
}
