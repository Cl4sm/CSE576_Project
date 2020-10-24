static int check_self_include(const char *key, struct lookup_context *ctxt)
{
	char *m_key, *m_base, *i_key, *i_base;

	/*
	 * If we are including a file map then check the
	 * full path of the map.
	 */
	if (*(key + 1) == '/') {
		if (!strcmp(key + 1, ctxt->mapname))
			return 1;
		else
			return 0;
	}

	i_key = strdup(key + 1);
	if (!i_key)
		return 0;
	i_base = basename(i_key);

	m_key = strdup(ctxt->mapname);
	if (!m_key) {
		free(i_key);
		return 0;
	}
	m_base = basename(m_key);

	if (!strcmp(m_base, i_base)) {
		free(i_key);
		free(m_key);
		return 1;
	}
	free(i_key);
	free(m_key);

	return 0;
}