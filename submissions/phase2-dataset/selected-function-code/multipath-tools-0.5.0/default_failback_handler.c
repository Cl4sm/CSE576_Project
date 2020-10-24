default_failback_handler(vector strvec)
{
	char * buff;

	buff = set_value(strvec);

	if (strlen(buff) == 6 && !strcmp(buff, "manual"))
		conf->pgfailback = -FAILBACK_MANUAL;
	else if (strlen(buff) == 9 && !strcmp(buff, "immediate"))
		conf->pgfailback = -FAILBACK_IMMEDIATE;
	else if (strlen(buff) == 10 && !strcmp(buff, "followover"))
		conf->pgfailback = -FAILBACK_FOLLOWOVER;
	else
		conf->pgfailback = atoi(buff);

	FREE(buff);

	return 0;
}
