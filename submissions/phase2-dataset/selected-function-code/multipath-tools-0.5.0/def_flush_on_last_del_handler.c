def_flush_on_last_del_handler(vector strvec)
{
	char * buff;

	buff = set_value(strvec);
	if (!buff)
		return 1;

	if ((strlen(buff) == 2 && strcmp(buff, "no") == 0) ||
	    (strlen(buff) == 1 && strcmp(buff, "0") == 0))
		conf->flush_on_last_del = FLUSH_DISABLED;
	else if ((strlen(buff) == 3 && strcmp(buff, "yes") == 0) ||
	    (strlen(buff) == 1 && strcmp(buff, "1") == 0))
		conf->flush_on_last_del = FLUSH_ENABLED;
	else
		conf->flush_on_last_del = FLUSH_UNDEF;

	FREE(buff);
	return 0;
}