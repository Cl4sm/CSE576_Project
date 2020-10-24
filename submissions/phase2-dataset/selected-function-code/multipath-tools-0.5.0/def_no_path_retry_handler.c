static int
def_no_path_retry_handler(vector strvec)
{
	char * buff;

	buff = set_value(strvec);
	if (!buff)
		return 1;

	if ((strlen(buff) == 4 && !strcmp(buff, "fail")) ||
	    (strlen(buff) == 1 && !strcmp(buff, "0")))
		conf->no_path_retry = NO_PATH_RETRY_FAIL;
	else if (strlen(buff) == 5 && !strcmp(buff, "queue"))
		conf->no_path_retry = NO_PATH_RETRY_QUEUE;
	else if ((conf->no_path_retry = atoi(buff)) < 1)
		conf->no_path_retry = NO_PATH_RETRY_UNDEF;

	FREE(buff);
	return 0;
}
