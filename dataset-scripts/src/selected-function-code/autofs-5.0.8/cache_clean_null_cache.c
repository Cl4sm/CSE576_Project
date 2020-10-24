void cache_clean_null_cache(struct mapent_cache *mc)
{
	struct mapent *me, *next;
	int i;

	for (i = 0; i < mc->size; i++) {
		me = mc->hash[i];
		if (me == NULL)
			continue;
		next = me->next;
		free(me->key);
		if (me->mapent)
			free(me->mapent);
		free(me);

		while (next != NULL) {
			me = next;
			next = me->next;
			free(me->key);
			free(me);
		}
		mc->hash[i] = NULL;
	}

	return;
}