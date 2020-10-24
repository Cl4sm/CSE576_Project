static int
update_multipath_table (struct multipath *mpp, vector pathvec)
{
	char params[PARAMS_SIZE] = {0};

	if (!mpp)
		return 1;

	if (dm_get_map(mpp->alias, &mpp->size, params)) {
		condlog(3, "%s: cannot get map", mpp->alias);
		return 1;
	}

	if (disassemble_map(pathvec, params, mpp)) {
		condlog(3, "%s: cannot disassemble map", mpp->alias);
		return 1;
	}

	return 0;
}
