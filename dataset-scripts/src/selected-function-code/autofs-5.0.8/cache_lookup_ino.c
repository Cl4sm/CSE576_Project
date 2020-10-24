struct mapent *cache_lookup_ino(struct mapent_cache *mc, dev_t dev, ino_t ino)
{
	struct mapent *me = NULL;
	struct list_head *head, *p;
	u_int32_t ino_index;

	ino_index_lock(mc);
	ino_index = ino_hash(dev, ino, mc->size);
	head = &mc->ino_index[ino_index];

	list_for_each(p, head) {
		me = list_entry(p, struct mapent, ino_index);

		if (me->dev != dev || me->ino != ino)
			continue;

		ino_index_unlock(mc);
		return me;
	}
	ino_index_unlock(mc);
	return NULL;
}