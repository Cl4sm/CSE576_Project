int
show_devices (char ** r, int * len, struct vectors *vecs)
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
		c += snprint_devices(c, maxlen, vecs);
		again = ((c - reply) == maxlen);
		REALLOC_REPLY(reply, again, maxlen);
	}

	*r = reply;
	*len = (int)(c - reply + 1);

	return 0;
}
