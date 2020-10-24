update_multipath_status (struct multipath *mpp)
{
	char status[PARAMS_SIZE] = {0};

	if (!mpp)
		return 1;

	if (dm_get_status(mpp->alias, status)) {
		condlog(3, "%s: cannot get status", mpp->alias);
		return 1;
	}

	if (disassemble_status(status, mpp)) {
		condlog(3, "%s: cannot disassemble status", mpp->alias);
		return 1;
	}

	return 0;
}
