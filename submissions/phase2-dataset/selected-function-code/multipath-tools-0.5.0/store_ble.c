extern int
store_ble (vector blist, char * str, int origin)
{
	struct blentry * ble;

	if (!str)
		return 0;

	if (!blist)
		goto out;

	ble = MALLOC(sizeof(struct blentry));

	if (!ble)
		goto out;

	if (regcomp(&ble->regex, str, REG_EXTENDED|REG_NOSUB))
		goto out1;

	if (!vector_alloc_slot(blist))
		goto out1;

	ble->str = str;
	ble->origin = origin;
	vector_set_slot(blist, ble);
	return 0;
out1:
	FREE(ble);
out:
	FREE(str);
	return 1;
}
