get_cmdvec (char * cmd, vector *v)
{
	int i;
	int r = 0;
	int get_param = 0;
	char * buff;
	struct key * kw = NULL;
	struct key * cmdkw = NULL;
	vector cmdvec, strvec;

	strvec = alloc_strvec(cmd);
	if (!strvec)
		return E_NOMEM;

	cmdvec = vector_alloc();

	if (!cmdvec) {
		free_strvec(strvec);
		return E_NOMEM;
	}

	vector_foreach_slot(strvec, buff, i) {
		if (*buff == '"')
			continue;
		if (get_param) {
			get_param = 0;
			cmdkw->param = strdup(buff);
			continue;
		}
		kw = find_key(buff);
		if (!kw) {
			r = E_SYNTAX;
			goto out;
		}
		cmdkw = alloc_key();
		if (!cmdkw) {
			r = E_NOMEM;
			goto out;
		}
		if (!vector_alloc_slot(cmdvec)) {
			FREE(cmdkw);
			r = E_NOMEM;
			goto out;
		}
		vector_set_slot(cmdvec, cmdkw);
		cmdkw->code = kw->code;
		cmdkw->has_param = kw->has_param;
		if (kw->has_param)
			get_param = 1;
	}
	if (get_param) {
		r = E_NOPARM;
		goto out;
	}
	*v = cmdvec;
	free_strvec(strvec);
	return 0;

out:
	free_strvec(strvec);
	free_keys(cmdvec);
	return r;
}
