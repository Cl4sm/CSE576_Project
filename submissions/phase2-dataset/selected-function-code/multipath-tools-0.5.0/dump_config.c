dump_config (void)
{
	char * c;
	char * reply;
	unsigned int maxlen = 256;
	int again = 1;

	reply = MALLOC(maxlen);

	while (again) {
		if (!reply)
			return 1;
		c = reply;
		c += snprint_defaults(c, reply + maxlen - c);
		again = ((c - reply) == maxlen);
		if (again) {
			reply = REALLOC(reply, maxlen *= 2);
			continue;
		}
		c += snprint_blacklist(c, reply + maxlen - c);
		again = ((c - reply) == maxlen);
		if (again) {
			reply = REALLOC(reply, maxlen *= 2);
			continue;
		}
		c += snprint_blacklist_except(c, reply + maxlen - c);
		again = ((c - reply) == maxlen);
		if (again) {
			reply = REALLOC(reply, maxlen *= 2);
			continue;
		}
		c += snprint_hwtable(c, reply + maxlen - c, conf->hwtable);
		again = ((c - reply) == maxlen);
		if (again) {
			reply = REALLOC(reply, maxlen *= 2);
			continue;
		}
		c += snprint_mptable(c, reply + maxlen - c, conf->mptable);
		again = ((c - reply) == maxlen);
		if (again)
			reply = REALLOC(reply, maxlen *= 2);
	}

	printf("%s", reply);
	FREE(reply);
	return 0;
}
