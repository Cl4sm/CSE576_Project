static int
max_fds_handler(vector strvec)
{
	char * buff;
	int r = 0, max_fds;

	buff = set_value(strvec);

	if (!buff)
		return 1;

	r = get_sys_max_fds(&max_fds);
	if (r) {
		/* Assume safe limit */
		max_fds = 4096;
	}
	if (strlen(buff) == 3 &&
	    !strcmp(buff, "max"))
		conf->max_fds = max_fds;
	else
		conf->max_fds = atoi(buff);

	if (conf->max_fds > max_fds)
		conf->max_fds = max_fds;

	FREE(buff);

	return r;
}
