int yp_all_callback(int status, char *ypkey, int ypkeylen,
		    char *val, int vallen, char *ypcb_data)
{
	struct callback_data *cbdata = (struct callback_data *) ypcb_data;
	struct autofs_point *ap = cbdata->ap;
	struct map_source *source = cbdata->source;
	struct mapent_cache *mc = source->mc;
	unsigned int logopt = cbdata->logopt;
	time_t age = cbdata->age;
	char *key, *mapent;
	int ret;

	if (status != YP_TRUE)
		return status;

	/* Ignore zero length and single non-printable char keys */
	if (ypkeylen == 0 || (ypkeylen == 1 && !isprint(*ypkey))) {
		warn(logopt, MODPREFIX
		     "ignoring invalid map entry, zero length or "
		     "single character non-printable key");
		return 0;
	}

	/*
	 * Ignore keys beginning with '+' as plus map
	 * inclusion is only valid in file maps.
	 */
	if (*ypkey == '+')
		return 0;

	key = sanitize_path(ypkey, ypkeylen, ap->type, ap->logopt);
	if (!key) {
		error(logopt, MODPREFIX "invalid path %s", ypkey);
		return 0;
	}

	mapent = alloca(vallen + 1);
	strncpy(mapent, val, vallen);
	*(mapent + vallen) = '\0';

	cache_writelock(mc);
	ret = cache_update(mc, source, key, mapent, age);
	cache_unlock(mc);

	free(key);

	if (ret == CHE_FAIL)
		return -1;

	return 0;
}