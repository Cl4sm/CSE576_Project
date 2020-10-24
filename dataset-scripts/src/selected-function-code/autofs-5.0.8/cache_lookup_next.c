struct mapent *cache_lookup_next(struct mapent_cache *mc, struct mapent *me)
{
	struct mapent *this;
	u_int32_t hashval;
	unsigned int i;

	if (!me)
		return NULL;

	this = me->next;
	while (this) {
		/* Multi mount entries are not primary */
		if (this->multi && this->multi != this) {
			this = this->next;
			continue;
		}
		return this;
	}

	hashval = hash(me->key, mc->size) + 1;
	if (hashval < mc->size) {
		for (i = (unsigned int) hashval; i < mc->size; i++) {
			this = mc->hash[i];
			if (!this)
				continue;

			while (this) {
				/* Multi mount entries are not primary */
				if (this->multi && this->multi != this) {
					this = this->next;
					continue;
				}
				return this;
			}
		}
	}
	return NULL;
}