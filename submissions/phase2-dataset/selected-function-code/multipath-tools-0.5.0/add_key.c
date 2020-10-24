static int
add_key (vector vec, char * str, unsigned long code, int has_param)
{
	struct key * kw;

	kw = alloc_key();

	if (!kw)
		return 1;

	kw->code = code;
	kw->has_param = has_param;
	kw->str = STRDUP(str);

	if (!kw->str)
		goto out;

	if (!vector_alloc_slot(vec))
		goto out1;

	vector_set_slot(vec, kw);

	return 0;

out1:
	FREE(kw->str);
out:
	FREE(kw);
	return 1;
}
