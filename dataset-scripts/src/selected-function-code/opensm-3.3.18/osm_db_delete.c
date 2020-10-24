int osm_db_delete(IN osm_db_domain_t * p_domain, IN char *p_key)
{
	osm_log_t *p_log = p_domain->p_db->p_log;
	osm_db_domain_imp_t *p_domain_imp =
	    (osm_db_domain_imp_t *) p_domain->p_domain_imp;
	char *p_prev_val = NULL;
	int res;

	OSM_LOG_ENTER(p_log);

	cl_spinlock_acquire(&p_domain_imp->lock);
	if (st_delete(p_domain_imp->p_hash,
		      (void *)&p_key, (void *)&p_prev_val)) {
		if (st_lookup(p_domain_imp->p_hash,
			      (st_data_t) p_key, (void *)&p_prev_val)) {
			OSM_LOG(p_log, OSM_LOG_ERROR,
				"key:%s still exists in:%s with value:%s\n",
				p_key, p_domain_imp->file_name, p_prev_val);
			res = 1;
		} else {
			free(p_key);
			free(p_prev_val);
			p_domain_imp->dirty = TRUE;
			res = 0;
		}
	} else {
		OSM_LOG(p_log, OSM_LOG_DEBUG,
			"fail to find key:%s. delete failed\n", p_key);
		res = 1;
	}
	cl_spinlock_release(&p_domain_imp->lock);

	OSM_LOG_EXIT(p_log);
	return res;
}