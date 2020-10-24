snprint_hw_no_path_retry (char * buff, int len, void * data)
{
	struct hwentry * hwe = (struct hwentry *)data;

	if (!hwe->no_path_retry)
		return 0;

	switch(hwe->no_path_retry) {
	case NO_PATH_RETRY_UNDEF:
		break;
	case NO_PATH_RETRY_FAIL:
		return snprintf(buff, len, "\"fail\"");
	case NO_PATH_RETRY_QUEUE:
		return snprintf(buff, len, "\"queue\"");
	default:
		return snprintf(buff, len, "%i",
				hwe->no_path_retry);
	}
	return 0;
}
