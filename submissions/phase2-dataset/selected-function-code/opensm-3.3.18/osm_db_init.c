int osm_db_init(IN osm_db_t * p_db, IN osm_log_t * p_log)
{
	osm_db_imp_t *p_db_imp;
	struct stat dstat;

	OSM_LOG_ENTER(p_log);

	p_db_imp = malloc(sizeof(osm_db_imp_t));
	if (!p_db_imp) {
		OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 6100: "
			"Failed to allocate db memory\n");
		return -1;
	}

	p_db_imp->db_dir_name = getenv("OSM_CACHE_DIR");
	if (!p_db_imp->db_dir_name || !(*p_db_imp->db_dir_name))
		p_db_imp->db_dir_name = OSM_DEFAULT_CACHE_DIR;

	/* Create the directory if it doesn't exist */
	/* There is a difference in creating directory between windows and linux */
#ifdef __WIN__
	{
		int ret;

		ret = SHCreateDirectoryEx(NULL, p_db_imp->db_dir_name, NULL);
		if (ret != ERROR_SUCCESS && ret != ERROR_ALREADY_EXISTS &&
			ret != ERROR_FILE_EXISTS)
			goto err;
	}
#else				/* __WIN__ */
	/* make sure the directory exists */
	if (lstat(p_db_imp->db_dir_name, &dstat)) {
		if (mkdir(p_db_imp->db_dir_name, 0755))
			goto err;
	}
#endif

	p_db->p_log = p_log;
	p_db->p_db_imp = (void *)p_db_imp;

	cl_list_init(&p_db->domains, 5);

	OSM_LOG_EXIT(p_log);

	return 0;

err:
	OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 6101: "
		"Failed to create the db directory:%s\n",
		p_db_imp->db_dir_name);
	free(p_db_imp);
	OSM_LOG_EXIT(p_log);
	return 1;
}