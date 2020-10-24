static int
mp_no_path_retry_handler(vector strvec)
{
	struct mpentry *mpe = VECTOR_LAST_SLOT(conf->mptable);
	char *buff;

	if (!mpe)
		return 1;

	buff = set_value(strvec);
	if (!buff)
		return 1;

	if ((strlen(buff) == 4 && !strcmp(buff, "fail")) ||
	    (strlen(buff) == 1 && !strcmp(buff, "0")))
		mpe->no_path_retry = NO_PATH_RETRY_FAIL;
	else if (strlen(buff) == 5 && !strcmp(buff, "queue"))
		mpe->no_path_retry = NO_PATH_RETRY_QUEUE;
	else if ((mpe->no_path_retry = atoi(buff)) < 1)
		mpe->no_path_retry = NO_PATH_RETRY_UNDEF;

	FREE(buff);
	return 0;
}
