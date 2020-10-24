int
show_map_topology (char ** r, int * len, struct multipath * mpp,
		   struct vectors * vecs)
{
	char * c;
	char * reply;
	unsigned int maxlen = INITIAL_REPLY_LEN;
	int again = 1;

	if (update_multipath(vecs, mpp->alias, 0))
		return 1;
	reply = MALLOC(maxlen);

	while (again) {
		if (!reply)
			return 1;

		c = reply;

		c += snprint_multipath_topology(c, reply + maxlen - c, mpp, 2);
		again = ((c - reply) == (maxlen - 1));

		REALLOC_REPLY(reply, again, maxlen);
	}
	*r = reply;
	*len = (int)(c - reply + 1);
	return 0;
}
