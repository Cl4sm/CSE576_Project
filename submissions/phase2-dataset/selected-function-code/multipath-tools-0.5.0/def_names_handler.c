static int
def_names_handler(vector strvec)
{
	char * buff;

	buff = set_value(strvec);

	if (!buff)
		return 1;

	if ((strlen(buff) == 2 && !strcmp(buff, "no")) ||
	    (strlen(buff) == 1 && !strcmp(buff, "0")))
		conf->user_friendly_names = USER_FRIENDLY_NAMES_OFF;
	else if ((strlen(buff) == 3 && !strcmp(buff, "yes")) ||
		 (strlen(buff) == 1 && !strcmp(buff, "1")))
		conf->user_friendly_names = USER_FRIENDLY_NAMES_ON;
	else
		conf->user_friendly_names = USER_FRIENDLY_NAMES_UNDEF;

	FREE(buff);
	return 0;
}
