int
add_handler (unsigned long fp, int (*fn)(void *, char **, int *, void *))
{
	struct handler * h;

	h = alloc_handler();

	if (!h)
		return 1;

	if (!vector_alloc_slot(handlers)) {
		FREE(h);
		return 1;
	}

	vector_set_slot(handlers, h);
	h->fingerprint = fp;
	h->fn = fn;

	return 0;
}
