def_weight_handler(vector strvec)
{
	char * buff;

	buff = set_value(strvec);

	if (!buff)
		return 1;

	if (strlen(buff) == 10 &&
	    !strcmp(buff, "priorities"))
		conf->rr_weight = RR_WEIGHT_PRIO;

	if (strlen(buff) == strlen("uniform") &&
	    !strcmp(buff, "uniform"))
		conf->rr_weight = RR_WEIGHT_NONE;

	FREE(buff);

	return 0;
}
