static int do_parse_mount(struct autofs_point *ap, struct map_source *source,
			  const char *name, int name_len, char *mapent,
			  struct lookup_context *ctxt)
{
	int ret;

	master_source_current_wait(ap->entry);
	ap->entry->current = source;

	ret = ctxt->parse->parse_mount(ap, name, name_len,
				 mapent, ctxt->parse->context);
	if (ret) {
		time_t now = time(NULL);
		struct mapent_cache *mc = source->mc;
		struct mapent *me;
		int rv = CHE_OK;

		cache_writelock(mc);
		me = cache_lookup_distinct(mc, name);
		if (!me)
			rv = cache_update(mc, source, name, NULL, now);
		if (rv != CHE_FAIL) {
			me = cache_lookup_distinct(mc, name);
			me->status = now + ap->negative_timeout;
		}
		cache_unlock(mc);
		return NSS_STATUS_TRYAGAIN;
	}
	return NSS_STATUS_SUCCESS;
}