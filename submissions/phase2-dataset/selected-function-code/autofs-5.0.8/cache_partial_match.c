struct mapent *cache_partial_match(struct mapent_cache *mc, const char *prefix)
{
	struct mapent *me = NULL;
	size_t len = strlen(prefix);
	unsigned int i;

	for (i = 0; i < mc->size; i++) {
		me = mc->hash[i];
		if (me == NULL)
			continue;

		if (len < strlen(me->key) &&
		    (strncmp(prefix, me->key, len) == 0) && me->key[len] == '/')
			return me;

		me = me->next;
		while (me != NULL) {
			if (len < strlen(me->key) &&
			    strncmp(prefix, me->key, len) == 0 && me->key[len] == '/')
				return me;
			me = me->next;
		}
	}
	return NULL;
}