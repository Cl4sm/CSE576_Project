vector_alloc_slot(vector v)
{
	void *new_slot = NULL;

	if (!v)
		return NULL;

	v->allocated += VECTOR_DEFAULT_SIZE;
	if (v->slot)
		new_slot = REALLOC(v->slot, sizeof (void *) * v->allocated);
	else
		new_slot = (void *) MALLOC(sizeof (void *) * v->allocated);

	if (!new_slot)
		v->allocated -= VECTOR_DEFAULT_SIZE;
	else
		v->slot = new_slot;

	return v->slot;
}
