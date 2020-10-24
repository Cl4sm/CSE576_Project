show_maps (char ** r, int *len, struct vectors * vecs, char * style)
{
	int i;
	struct multipath * mpp;
	char * c;
	char * reply;
	unsigned int maxlen = INITIAL_REPLY_LEN;
	int again = 1;

	get_multipath_layout(vecs->mpvec, 1);
	reply = MALLOC(maxlen);

	while (again) {
		if (!reply)
			return 1;

		c = reply;
		if (VECTOR_SIZE(vecs->mpvec) > 0)
			c += snprint_multipath_header(c, reply + maxlen - c,
						      style);

		vector_foreach_slot(vecs->mpvec, mpp, i)
			c += snprint_multipath(c, reply + maxlen - c,
					       style, mpp);

		again = ((c - reply) == (maxlen - 1));

		REALLOC_REPLY(reply, again, maxlen);
	}
	*r = reply;
	*len = (int)(c - reply + 1);
	return 0;
}
