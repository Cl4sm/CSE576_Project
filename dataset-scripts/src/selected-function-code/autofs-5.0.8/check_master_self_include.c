static int check_master_self_include(struct master *master, struct lookup_context *ctxt)
{
	char *m_path, *m_base, *i_path, *i_base;

	/*
	 * If we are including a file map then check the
	 * full path of the map.
	 */
	if (*master->name == '/') {
		if (!strcmp(master->name, ctxt->mapname))
			return 1;
		else
			return 0;
	}

	/* Otherwise only check the map name itself. */

	i_path = strdup(ctxt->mapname);
	if (!i_path)
		return 0;
	i_base = basename(i_path);

	m_path = strdup(master->name);
	if (!m_path) {
		free(i_path);
		return 0;
	}
	m_base = basename(m_path);

	if (!strcmp(m_base, i_base)) {
		free(i_path);
		free(m_path);
		return  1;
	}
	free(i_path);
	free(m_path);

	return 0;
}