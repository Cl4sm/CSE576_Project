static int
update_offset_entry(struct autofs_point *ap, const char *name,
		    const char *m_root, int m_root_len,
		    const char *path, const char *myoptions, const char *loc,
		    time_t age)
{
	struct map_source *source;
	struct mapent_cache *mc;
	char m_key[PATH_MAX + 1];
	char m_mapent[MAPENT_MAX_LEN + 1];
	int p_len, m_key_len, m_options_len, m_mapent_len;
	int ret;

	source = ap->entry->current;
	ap->entry->current = NULL;
	master_source_current_signal(ap->entry);

	mc = source->mc;

	/* Internal hosts map may have loc == NULL */
	if (!*path) {
		error(ap->logopt,
		      MODPREFIX "syntax error in offset %s -> %s", path, loc);
		return CHE_FAIL;
	}

	p_len = strlen(path);
	/* Trailing '/' causes us pain */
	if (p_len > 1) {
		while (p_len > 1 && path[p_len - 1] == '/')
			p_len--;
	}
	m_key_len = m_root_len + p_len;
	if (m_key_len > PATH_MAX) {
		error(ap->logopt, MODPREFIX "multi mount key too long");
		return CHE_FAIL;
	}
	strcpy(m_key, m_root);
	strncat(m_key, path, p_len);
	m_key[m_key_len] = '\0';

	m_options_len = 0;
	if (*myoptions)
		m_options_len = strlen(myoptions) + 2;

	m_mapent_len = strlen(loc);
	if (m_mapent_len + m_options_len > MAPENT_MAX_LEN) {
		error(ap->logopt, MODPREFIX "multi mount mapent too long");
		return CHE_FAIL;
	}

	if (*myoptions) {
		strcpy(m_mapent, "-");
		strcat(m_mapent, myoptions);
		if (loc) {
			strcat(m_mapent, " ");
			strcat(m_mapent, loc);
		}
	} else
		strcpy(m_mapent, loc);

	ret = cache_update_offset(mc, name, m_key, m_mapent, age);
	if (ret == CHE_DUPLICATE)
		warn(ap->logopt, MODPREFIX
		     "syntax error or duplicate offset %s -> %s", path, loc);
	else if (ret == CHE_FAIL)
		debug(ap->logopt, MODPREFIX
		      "failed to update multi-mount offset %s -> %s", path, m_mapent);
	else {
		ret = CHE_OK;
		debug(ap->logopt, MODPREFIX
		      "updated multi-mount offset %s -> %s", path, m_mapent);
	}

	return ret;
}