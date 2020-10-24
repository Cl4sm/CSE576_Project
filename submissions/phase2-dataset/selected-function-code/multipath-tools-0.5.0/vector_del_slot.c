vector_del_slot(vector v, int slot)
{
	int i;

	if (!v || !v->allocated || slot < 0 || slot > VECTOR_SIZE(v))
		return;

	for (i = slot + 1; i < VECTOR_SIZE(v); i++)
		v->slot[i-1] = v->slot[i];

	v->allocated -= VECTOR_DEFAULT_SIZE;

	if (v->allocated <= 0) {
		FREE(v->slot);
		v->slot = NULL;
		v->allocated = 0;
	} else {
		void *new_slot;

		new_slot = REALLOC(v->slot, sizeof (void *) * v->allocated);
		if (!new_slot)
			v->allocated += VECTOR_DEFAULT_SIZE;
		else
			v->slot = new_slot;
	}
}
