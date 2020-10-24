def_retain_hwhandler_handler(vector strvec)
{
	char * buff;

	buff = set_value(strvec);

	if (!buff)
		return 1;

	if ((strlen(buff) == 2 && !strcmp(buff, "no")) ||
	    (strlen(buff) == 1 && !strcmp(buff, "0")))
		conf->retain_hwhandler = RETAIN_HWHANDLER_OFF;
	else if ((strlen(buff) == 3 && !strcmp(buff, "yes")) ||
		 (strlen(buff) == 1 && !strcmp(buff, "1")))
		conf->retain_hwhandler = RETAIN_HWHANDLER_ON;
	else
		conf->retain_hwhandler = RETAIN_HWHANDLER_UNDEF;

	FREE(buff);
	return 0;
}
