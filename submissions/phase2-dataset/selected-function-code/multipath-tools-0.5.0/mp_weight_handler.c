static int
mp_weight_handler(vector strvec)
{
	struct mpentry * mpe = VECTOR_LAST_SLOT(conf->mptable);
	char * buff;

	if (!mpe)
		return 1;

	buff = set_value(strvec);

	if (!buff)
		return 1;

	if (strlen(buff) == 10 &&
	    !strcmp(buff, "priorities"))
		mpe->rr_weight = RR_WEIGHT_PRIO;

	if (strlen(buff) == strlen("uniform") &&
	    !strcmp(buff, "uniform"))
		mpe->rr_weight = RR_WEIGHT_NONE;

	FREE(buff);

	return 0;
}
