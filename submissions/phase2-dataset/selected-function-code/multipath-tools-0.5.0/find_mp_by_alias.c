find_mp_by_alias (vector mpvec, char * alias)
{
	int i;
	int len;
	struct multipath * mpp;

	if (!mpvec)
		return NULL;

	len = strlen(alias);

	if (!len)
		return NULL;

	vector_foreach_slot (mpvec, mpp, i) {
		if (strlen(mpp->alias) == len &&
		    !strncmp(mpp->alias, alias, len))
			return mpp;
	}
	return NULL;
}
