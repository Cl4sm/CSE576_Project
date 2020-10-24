mp_mode_handler(vector strvec)
{
	mode_t mode;
	struct mpentry *mpe = VECTOR_LAST_SLOT(conf->mptable);
	char *buff;

	if (!mpe)
		return 1;

	buff = set_value(strvec);
	if (!buff)
		return 1;
	if (sscanf(buff, "%o", &mode) == 1 && mode <= 0777) {
		mpe->attribute_flags |= (1 << ATTR_MODE);
		mpe->mode = mode;
	}

	FREE(buff);
	return 0;
}
