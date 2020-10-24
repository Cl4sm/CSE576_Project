static int
mp_failback_handler(vector strvec)
{
	struct mpentry * mpe = VECTOR_LAST_SLOT(conf->mptable);
	char * buff;

	if (!mpe)
		return 1;

	buff = set_value(strvec);

	if (strlen(buff) == 6 && !strcmp(buff, "manual"))
		mpe->pgfailback = -FAILBACK_MANUAL;
	else if (strlen(buff) == 9 && !strcmp(buff, "immediate"))
		mpe->pgfailback = -FAILBACK_IMMEDIATE;
	else if (strlen(buff) == 10 && !strcmp(buff, "followover"))
		mpe->pgfailback = -FAILBACK_FOLLOWOVER;
	else
		mpe->pgfailback = atoi(buff);

	FREE(buff);

	return 0;
}
