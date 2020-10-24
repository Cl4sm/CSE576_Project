int osm_db_update(IN osm_db_domain_t * p_domain, IN char *p_key, IN char *p_val)
{
	osm_log_t *p_log = p_domain->p_db->p_log;
	osm_db_domain_imp_t *p_domain_imp =
	    (osm_db_domain_imp_t *) p_domain->p_domain_imp;
	char *p_prev_val = NULL;
	char *p_new_key;
	char *p_new_val;

	cl_spinlock_acquire(&p_domain_imp->lock);

	if (st_lookup(p_domain_imp->p_hash,
		      (st_data_t) p_key, (void *)&p_prev_val)) {
		OSM_LOG(p_log, OSM_LOG_DEBUG,
			"Key:%s previously exists in:%s with value:%s\n",
			p_key, p_domain_imp->file_name, p_prev_val);
		p_new_key = p_key;
		/* same key, same value - nothing to update */
		if (p_prev_val && !strcmp(p_val, p_prev_val))
			goto Exit;
	} else {
		/* need to allocate the key */
		p_new_key = malloc(sizeof(char) * (strlen(p_key) + 1));
		strcpy(p_new_key, p_key);
	}

	/* need to arange a new copy of the  value */
	p_new_val = malloc(sizeof(char) * (strlen(p_val) + 1));
	strcpy(p_new_val, p_val);

	st_insert(p_domain_imp->p_hash, (st_data_t) p_new_key,
		  (st_data_t) p_new_val);

	if (p_prev_val)
		free(p_prev_val);

	p_domain_imp->dirty = TRUE;

Exit:
	cl_spinlock_release(&p_domain_imp->lock);

	return 0;
}