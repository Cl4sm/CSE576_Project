cli_suspend(void * v, char ** reply, int * len, void * data)
{
	struct vectors * vecs = (struct vectors *)data;
	char * param = get_keyparam(v, MAP);
	int r = dm_simplecmd_noflush(DM_DEVICE_SUSPEND, param);

	param = convert_dev(param, 0);
	condlog(2, "%s: suspend (operator)", param);

	if (!r) /* error */
		return 1;

	struct multipath * mpp = find_mp_by_alias(vecs->mpvec, param);

	if (!mpp)
		return 1;

	dm_get_info(param, &mpp->dmi);
	return 0;
}
