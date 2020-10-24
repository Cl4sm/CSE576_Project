static int lookup_one(struct autofs_point *ap,
		      const char *key, int key_len,
		      struct lookup_context *ctxt)
{
	struct map_source *source;
	struct mapent_cache *mc;
	char *tablename;
	nis_result *result;
	nis_object *this;
	char *mapent;
	time_t age = time(NULL);
	int ret, cur_state;
	char buf[MAX_ERR_BUF];

	source = ap->entry->current;
	ap->entry->current = NULL;
	master_source_current_signal(ap->entry);

	mc = source->mc;

	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cur_state);
	tablename = malloc(strlen(key) + strlen(ctxt->mapname) +
			   strlen(ctxt->domainname) + 20);
	if (!tablename) {
		char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
		logerr(MODPREFIX "malloc: %s", estr);
		pthread_setcancelstate(cur_state, NULL);
		return -1;
	}
	sprintf(tablename, "[key=%s],%s.org_dir.%s", key, ctxt->mapname,
		ctxt->domainname);

	result = nis_list(tablename, FOLLOW_PATH | FOLLOW_LINKS, NULL, NULL);
	if (result->status != NIS_SUCCESS && result->status != NIS_S_SUCCESS) {
		nis_error rs = result->status;
		nis_freeresult(result);
		free(tablename);
		pthread_setcancelstate(cur_state, NULL);
		if (rs == NIS_NOTFOUND ||
		    rs == NIS_S_NOTFOUND ||
		    rs == NIS_PARTIAL)
			return CHE_MISSING;

		return -rs;
	}

	
	this = NIS_RES_OBJECT(result);
	mapent = ENTRY_VAL(this, 1);
	cache_writelock(mc);
	ret = cache_update(mc, source, key, mapent, age);
	cache_unlock(mc);

	nis_freeresult(result);
	free(tablename);
	pthread_setcancelstate(cur_state, NULL);

	return ret;
}