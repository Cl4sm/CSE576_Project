struct hwentry *
find_hwe (vector hwtable, char * vendor, char * product, char * revision)
{
	int i;
	struct hwentry hwe, *tmp, *ret = NULL;

	hwe.vendor = vendor;
	hwe.product = product;
	hwe.revision = revision;
	/*
	 * Search backwards here.
	 * User modified entries are attached at the end of
	 * the list, so we have to check them first before
	 * continuing to the generic entries
	 */
	vector_foreach_slot_backwards (hwtable, tmp, i) {
		if (hwe_regmatch(tmp, &hwe))
			continue;
		ret = tmp;
		break;
	}
	return ret;
}
