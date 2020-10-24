int lookup_read_master(struct master *master, time_t age, void *context)
{
	struct lookup_context *ctxt = (struct lookup_context *) context;
	unsigned int timeout = master->default_timeout;
	unsigned int logging = master->default_logging;
	unsigned int logopt =  master->logopt;
	char *tablename;
	nis_result *result;
	nis_object *this;
	unsigned int current, result_count;
	char *path, *ent;
	char *buffer;
	char buf[MAX_ERR_BUF];
	int cur_state, len;

	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cur_state);
	tablename = malloc(strlen(ctxt->mapname) + strlen(ctxt->domainname) + 20);
	if (!tablename) {
		char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
		logerr(MODPREFIX "malloc: %s", estr);
		pthread_setcancelstate(cur_state, NULL);
		return NSS_STATUS_UNAVAIL;
	}
	sprintf(tablename, "%s.org_dir.%s", ctxt->mapname, ctxt->domainname);

	/* check that the table exists */
	result = nis_lookup(tablename, FOLLOW_PATH | FOLLOW_LINKS);
	if (result->status != NIS_SUCCESS && result->status != NIS_S_SUCCESS) {
		nis_freeresult(result);
		crit(logopt,
		     MODPREFIX "couldn't locate nis+ table %s", ctxt->mapname);
		free(tablename);
		pthread_setcancelstate(cur_state, NULL);
		return NSS_STATUS_NOTFOUND;
	}

	sprintf(tablename, "[],%s.org_dir.%s", ctxt->mapname, ctxt->domainname);

	result = nis_list(tablename, FOLLOW_PATH | FOLLOW_LINKS, NULL, NULL);
	if (result->status != NIS_SUCCESS && result->status != NIS_S_SUCCESS) {
		nis_freeresult(result);
		crit(logopt,
		     MODPREFIX "couldn't enumrate nis+ map %s", ctxt->mapname);
		free(tablename);
		pthread_setcancelstate(cur_state, NULL);
		return NSS_STATUS_UNAVAIL;
	}

	current = 0;
	result_count = NIS_RES_NUMOBJ(result);

	while (result_count--) {
		this = &result->objects.objects_val[current++];
		path = ENTRY_VAL(this, 0);
		/*
		 * Ignore keys beginning with '+' as plus map
		 * inclusion is only valid in file maps.
		 */
		if (*path == '+')
			continue;

		ent = ENTRY_VAL(this, 1);

		len = ENTRY_LEN(this, 0) + 1 + ENTRY_LEN(this, 1) + 2;
		buffer = malloc(len);
		if (!buffer) {
			logerr(MODPREFIX "could not malloc parse buffer");
			continue;
		}
		memset(buffer, 0, len);

		strcat(buffer, path);
		strcat(buffer, " ");
		strcat(buffer, ent);

		master_parse_entry(buffer, timeout, logging, age);

		free(buffer);
	}

	nis_freeresult(result);
	free(tablename);
	pthread_setcancelstate(cur_state, NULL);

	return NSS_STATUS_SUCCESS;
}