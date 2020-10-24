static int
snprint_mp_no_path_retry (char * buff, int len, void * data)
{
	struct mpentry * mpe = (struct mpentry *)data;

	if (!mpe->no_path_retry)
		return 0;

	switch(mpe->no_path_retry) {
	case NO_PATH_RETRY_UNDEF:
		break;
	case NO_PATH_RETRY_FAIL:
		return snprintf(buff, len, "\"fail\"");
	case NO_PATH_RETRY_QUEUE:
		return snprintf(buff, len, "\"queue\"");
	default:
		return snprintf(buff, len, "%i",
				mpe->no_path_retry);
	}
	return 0;
}
