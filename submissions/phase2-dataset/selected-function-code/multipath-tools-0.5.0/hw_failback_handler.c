hw_failback_handler(vector strvec)
{
	struct hwentry * hwe = VECTOR_LAST_SLOT(conf->hwtable);
	char * buff;

	if (!hwe)
		return 1;

	buff = set_value(strvec);

	if (strlen(buff) == 6 && !strcmp(buff, "manual"))
		hwe->pgfailback = -FAILBACK_MANUAL;
	else if (strlen(buff) == 9 && !strcmp(buff, "immediate"))
		hwe->pgfailback = -FAILBACK_IMMEDIATE;
	else if (strlen(buff) == 10 && !strcmp(buff, "followover"))
		hwe->pgfailback = -FAILBACK_FOLLOWOVER;
	else
		hwe->pgfailback = atoi(buff);

	FREE(buff);

	return 0;
}
