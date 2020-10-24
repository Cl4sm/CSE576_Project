osm_db_domain_t *osm_db_domain_init(IN osm_db_t * p_db, IN const char *domain_name)
{
	osm_db_domain_t *p_domain;
	osm_db_domain_imp_t *p_domain_imp;
	size_t path_len;
	osm_log_t *p_log = p_db->p_log;
	FILE *p_file;

	OSM_LOG_ENTER(p_log);

	/* allocate a new domain object */
	p_domain = malloc(sizeof(osm_db_domain_t));
	if (p_domain == NULL) {
		OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 610C: "
			"Failed to allocate domain memory\n");
		goto Exit;
	}

	p_domain_imp = malloc(sizeof(osm_db_domain_imp_t));
	if (p_domain_imp == NULL) {
		OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 610D: "
			"Failed to allocate domain_imp memory\n");
		free(p_domain);
		p_domain = NULL;
		goto Exit;
	}

	path_len = strlen(((osm_db_imp_t *) p_db->p_db_imp)->db_dir_name)
	    + strlen(domain_name) + 2;

	/* set the domain file name */
	p_domain_imp->file_name = malloc(path_len);
	if (p_domain_imp->file_name == NULL) {
		OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 610E: "
			"Failed to allocate file_name memory\n");
		free(p_domain_imp);
		free(p_domain);
		p_domain = NULL;
		goto Exit;
	}
	snprintf(p_domain_imp->file_name, path_len, "%s/%s",
		 ((osm_db_imp_t *) p_db->p_db_imp)->db_dir_name, domain_name);

	/* make sure the file exists - or exit if not writable */
	p_file = fopen(p_domain_imp->file_name, "a+");
	if (!p_file) {
		OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 6102: "
			"Failed to open the db file:%s\n",
			p_domain_imp->file_name);
		free(p_domain_imp);
		free(p_domain);
		p_domain = NULL;
		goto Exit;
	}
	fclose(p_file);

	/* initialize the hash table object */
	p_domain_imp->p_hash = st_init_strtable();
	CL_ASSERT(p_domain_imp->p_hash != NULL);
	p_domain_imp->dirty = FALSE;

	p_domain->p_db = p_db;
	cl_list_insert_tail(&p_db->domains, p_domain);
	p_domain->p_domain_imp = p_domain_imp;
	cl_spinlock_construct(&p_domain_imp->lock);
	cl_spinlock_init(&p_domain_imp->lock);

Exit:
	OSM_LOG_EXIT(p_log);
	return p_domain;
}