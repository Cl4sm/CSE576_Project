mp_flush_on_last_del_handler(vector strvec)
{
	struct mpentry *mpe = VECTOR_LAST_SLOT(conf->mptable);
	char * buff;

	if (!mpe)
		return 1;

	buff = set_value(strvec);
	if (!buff)
		return 1;

	if ((strlen(buff) == 2 && strcmp(buff, "no") == 0) ||
	    (strlen(buff) == 1 && strcmp(buff, "0") == 0))
		mpe->flush_on_last_del = FLUSH_DISABLED;
	else if ((strlen(buff) == 3 && strcmp(buff, "yes") == 0) ||
	    (strlen(buff) == 1 && strcmp(buff, "1") == 0))
		mpe->flush_on_last_del = FLUSH_ENABLED;
	else
		mpe->flush_on_last_del = FLUSH_UNDEF;

	FREE(buff);
	return 0;
}
