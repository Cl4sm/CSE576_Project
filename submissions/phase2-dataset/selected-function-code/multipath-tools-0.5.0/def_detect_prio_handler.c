static int
def_detect_prio_handler(vector strvec)
{
	char * buff;

	buff = set_value(strvec);

	if (!buff)
		return 1;

	if ((strlen(buff) == 2 && !strcmp(buff, "no")) ||
	    (strlen(buff) == 1 && !strcmp(buff, "0")))
		conf->detect_prio = DETECT_PRIO_OFF;
	else if ((strlen(buff) == 3 && !strcmp(buff, "yes")) ||
		 (strlen(buff) == 1 && !strcmp(buff, "1")))
		conf->detect_prio = DETECT_PRIO_ON;
	else
		conf->detect_prio = DETECT_PRIO_UNDEF;

	FREE(buff);
	return 0;
}
