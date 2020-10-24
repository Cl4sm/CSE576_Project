int lookup_read_map(struct autofs_point *ap, time_t age, void *context)
{
	struct lookup_context *ctxt = (struct lookup_context *) context;
	struct map_source *source;
	struct mapent_cache *mc;
	char *tablename;
	nis_result *result;
	nis_object *this;
	unsigned int current, result_count;
	char *key, *mapent;
	char buf[MAX_ERR_BUF];
	int cur_state;

	source = ap->entry->current;
	ap->entry->current = NULL;
	master_source_current_signal(ap->entry);

	/*
	 * If we don't need to create directories then there's no use
	 * reading the map. We always need to read the whole map for
	 * direct mounts in order to mount the triggers.
	 */
	if (!(ap->flags & MOUNT_FLAG_GHOST) && ap->type != LKP_DIRECT) {
		debug(ap->logopt, "map read not needed, so not done");
		return NSS_STATUS_SUCCESS;
	}

	mc = source->mc;

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
		crit(ap->logopt,
		     MODPREFIX "couldn't locate nis+ table %s", ctxt->mapname);
		free(tablename);
		pthread_setcancelstate(cur_state, NULL);
		return NSS_STATUS_NOTFOUND;
	}

	sprintf(tablename, "[],%s.org_dir.%s", ctxt->mapname, ctxt->domainname);

	result = nis_list(tablename, FOLLOW_PATH | FOLLOW_LINKS, NULL, NULL);
	if (result->status != NIS_SUCCESS && result->status != NIS_S_SUCCESS) {
		nis_freeresult(result);
		crit(ap->logopt,
		     MODPREFIX "couldn't enumrate nis+ map %s", ctxt->mapname);
		free(tablename);
		pthread_setcancelstate(cur_state, NULL);
		return NSS_STATUS_UNAVAIL;
	}

	current = 0;
	result_count = NIS_RES_NUMOBJ(result);

	while (result_count--) {
		char *s_key;
		size_t len;

		this = &result->objects.objects_val[current++];
		key = ENTRY_VAL(this, 0);
		len = ENTRY_LEN(this, 0);

		/*
		 * Ignore keys beginning with '+' as plus map
		 * inclusion is only valid in file maps.
		 */
		if (*key == '+')
			continue;

		s_key = sanitize_path(key, len, ap->type, ap->logopt);
		if (!s_key)
			continue;

		mapent = ENTRY_VAL(this, 1);

		cache_writelock(mc);
		cache_update(mc, source, s_key, mapent, age);
		cache_unlock(mc);

		free(s_key);
	}

	nis_freeresult(result);

	source->age = age;

	free(tablename);
	pthread_setcancelstate(cur_state, NULL);

	return NSS_STATUS_SUCCESS;
}