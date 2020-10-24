int
parse_cmd (char * cmd, char ** reply, int * len, void * data)
{
	int r;
	struct handler * h;
	vector cmdvec = NULL;

	r = get_cmdvec(cmd, &cmdvec);

	if (r) {
		*reply = genhelp_handler();
		*len = strlen(*reply) + 1;
		return 0;
	}

	h = find_handler(fingerprint(cmdvec));

	if (!h || !h->fn) {
		*reply = genhelp_handler();
		*len = strlen(*reply) + 1;
		free_keys(cmdvec);
		return 0;
	}

	/*
	 * execute handler
	 */
	r = h->fn(cmdvec, reply, len, data);
	free_keys(cmdvec);

	return r;
}
