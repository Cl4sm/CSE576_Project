static int
hw_weight_handler(vector strvec)
{
	struct hwentry * hwe = VECTOR_LAST_SLOT(conf->hwtable);
	char * buff;

	if (!hwe)
		return 1;

	buff = set_value(strvec);

	if (!buff)
		return 1;

	if (strlen(buff) == 10 &&
	    !strcmp(buff, "priorities"))
		hwe->rr_weight = RR_WEIGHT_PRIO;

	if (strlen(buff) == strlen("uniform") &&
	    !strcmp(buff, "uniform"))
		hwe->rr_weight = RR_WEIGHT_NONE;

	FREE(buff);

	return 0;
}
