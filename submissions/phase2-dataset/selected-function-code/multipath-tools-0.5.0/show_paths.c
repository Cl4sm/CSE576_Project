show_paths (char ** r, int * len, struct vectors * vecs, char * style)
{
	int i;
	struct path * pp;
	char * c;
	char * reply;
	unsigned int maxlen = INITIAL_REPLY_LEN;
	int again = 1;

	get_path_layout(vecs->pathvec, 1);
	reply = MALLOC(maxlen);

	while (again) {
		if (!reply)
			return 1;

		c = reply;

		if (VECTOR_SIZE(vecs->pathvec) > 0)
			c += snprint_path_header(c, reply + maxlen - c,
						 style);

		vector_foreach_slot(vecs->pathvec, pp, i)
			c += snprint_path(c, reply + maxlen - c,
					  style, pp);

		again = ((c - reply) == (maxlen - 1));

		REALLOC_REPLY(reply, again, maxlen);
	}
	*r = reply;
	*len = (int)(c - reply + 1);
	return 0;
}
