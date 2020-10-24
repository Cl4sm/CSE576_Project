struct mapent *cache_lookup_first(struct mapent_cache *mc)
{
	struct mapent *me = NULL;
	unsigned int i;

	for (i = 0; i < mc->size; i++) {
		me = mc->hash[i];
		if (!me)
			continue;

		while (me) {
			/* Multi mount entries are not primary */
			if (me->multi && me->multi != me) {
				me = me->next;
				continue;
			}
			return me;
		}
	}
	return NULL;
}