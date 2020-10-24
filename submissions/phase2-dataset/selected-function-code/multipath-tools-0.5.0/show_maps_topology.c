show_maps_topology (char ** r, int * len, struct vectors * vecs)
{
	int i;
	struct multipath * mpp;
	char * c;
	char * reply;
	unsigned int maxlen = INITIAL_REPLY_LEN;
	int again = 1;
 
	get_path_layout(vecs->pathvec, 0);
	reply = MALLOC(maxlen);

	while (again) {
		if (!reply)
			return 1;

		c = reply;

		vector_foreach_slot(vecs->mpvec, mpp, i) {
			if (update_multipath(vecs, mpp->alias, 0)) {
				i--;
				continue;
			}
			c += snprint_multipath_topology(c, reply + maxlen - c,
							mpp, 2);
		}

		again = ((c - reply) == (maxlen - 1));

		REALLOC_REPLY(reply, again, maxlen);
	}
	*r = reply;
	*len = (int)(c - reply + 1);
	return 0;
}
