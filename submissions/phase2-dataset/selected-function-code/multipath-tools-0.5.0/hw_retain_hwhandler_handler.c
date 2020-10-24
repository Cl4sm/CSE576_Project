hw_retain_hwhandler_handler(vector strvec)
{
	struct hwentry *hwe = VECTOR_LAST_SLOT(conf->hwtable);
	char * buff;

	if (!hwe)
		return 1;

	buff = set_value(strvec);

	if (!buff)
		return 1;

	if ((strlen(buff) == 2 && !strcmp(buff, "no")) ||
	    (strlen(buff) == 1 && !strcmp(buff, "0")))
		hwe->retain_hwhandler = RETAIN_HWHANDLER_OFF;
	else if ((strlen(buff) == 3 && !strcmp(buff, "yes")) ||
		 (strlen(buff) == 1 && !strcmp(buff, "1")))
		hwe->retain_hwhandler = RETAIN_HWHANDLER_ON;
	else
		hwe->user_friendly_names = RETAIN_HWHANDLER_UNDEF;

	FREE(buff);
	return 0;
}
