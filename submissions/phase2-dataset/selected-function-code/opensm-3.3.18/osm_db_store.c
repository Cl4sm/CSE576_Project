int osm_db_store(IN osm_db_domain_t * p_domain,
		 IN boolean_t fsync_high_avail_files)
{
	osm_log_t *p_log = p_domain->p_db->p_log;
	osm_db_domain_imp_t *p_domain_imp;
	FILE *p_file = NULL;
	int fd, status = 0;
	char *p_tmp_file_name = NULL;

	OSM_LOG_ENTER(p_log);

	p_domain_imp = (osm_db_domain_imp_t *) p_domain->p_domain_imp;

	p_tmp_file_name = malloc(sizeof(char) *
				 (strlen(p_domain_imp->file_name) + 8));
	if (!p_tmp_file_name) {
		OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 6113: "
			"Failed to allocate memory for temporary file name\n");
		goto Exit2;
	}
	strcpy(p_tmp_file_name, p_domain_imp->file_name);
	strcat(p_tmp_file_name, ".tmp");

	cl_spinlock_acquire(&p_domain_imp->lock);

	if (p_domain_imp->dirty == FALSE)
		goto Exit;

	/* open up the output file */
	p_file = fopen(p_tmp_file_name, "w");
	if (!p_file) {
		OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 6107: "
			"Failed to open the db file:%s for writing: err:%s\n",
			p_domain_imp->file_name, strerror(errno));
		status = 1;
		goto Exit;
	}

	st_foreach(p_domain_imp->p_hash, dump_tbl_entry, (st_data_t) p_file);

	if (fsync_high_avail_files) {
		if (fflush(p_file) == 0) {
			fd = fileno(p_file);
			if (fd != -1) {
				if (fsync(fd) == -1)
					OSM_LOG(p_log, OSM_LOG_ERROR,
						"ERR 6110: fsync() failed (%s) for %s\n",
						strerror(errno),
						p_domain_imp->file_name);
			} else
				OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 6111: "
					"fileno() failed for %s\n",
					p_domain_imp->file_name);
		} else
			OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 6112: "
				"fflush() failed (%s) for %s\n",
				strerror(errno), p_domain_imp->file_name);
	}

	fclose(p_file);

	status = rename(p_tmp_file_name, p_domain_imp->file_name);
	if (status) {
		OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 6108: "
			"Failed to rename the db file to:%s (err:%s)\n",
			p_domain_imp->file_name, strerror(errno));
		goto Exit;
	}
	p_domain_imp->dirty = FALSE;
Exit:
	cl_spinlock_release(&p_domain_imp->lock);
	free(p_tmp_file_name);
Exit2:
	OSM_LOG_EXIT(p_log);
	return status;
}