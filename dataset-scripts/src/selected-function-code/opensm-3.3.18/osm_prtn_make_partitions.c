ib_api_status_t osm_prtn_make_partitions(osm_log_t * p_log, osm_subn_t * p_subn)
{
	struct stat statbuf;
	const char *file_name;
	boolean_t is_config = TRUE;
	boolean_t is_wrong_config = FALSE;
	ib_api_status_t status = IB_SUCCESS;
	cl_map_item_t *p_next;
	osm_prtn_t *p;

	file_name = p_subn->opt.partition_config_file ?
	    p_subn->opt.partition_config_file : OSM_DEFAULT_PARTITION_CONFIG_FILE;
	if (stat(file_name, &statbuf)) {
		OSM_LOG(p_log, OSM_LOG_VERBOSE, "Partition configuration "
			"%s is not accessible (%s)\n", file_name,
			strerror(errno));
		is_config = FALSE;
	}

retry_default:
	/* clean up current port maps */
	p_next = cl_qmap_head(&p_subn->prtn_pkey_tbl);
	while (p_next != cl_qmap_end(&p_subn->prtn_pkey_tbl)) {
		p = (osm_prtn_t *) p_next;
		p_next = cl_qmap_next(&p->map_item);
		cl_map_remove_all(&p->part_guid_tbl);
		cl_map_remove_all(&p->full_guid_tbl);
	}

	global_pkey_counter = 0;

	status = prtn_make_default(p_log, p_subn, !is_config);
	if (status != IB_SUCCESS)
		goto _err;

	if (is_config && osm_prtn_config_parse_file(p_log, p_subn, file_name)) {
		OSM_LOG(p_log, OSM_LOG_VERBOSE, "Partition configuration "
			"was not fully processed\n");
		is_wrong_config = TRUE;
	}

	/* and now clean up empty partitions */
	p_next = cl_qmap_head(&p_subn->prtn_pkey_tbl);
	while (p_next != cl_qmap_end(&p_subn->prtn_pkey_tbl)) {
		p = (osm_prtn_t *) p_next;
		p_next = cl_qmap_next(&p->map_item);
		if (cl_map_count(&p->part_guid_tbl) == 0 &&
		    cl_map_count(&p->full_guid_tbl) == 0) {
			cl_qmap_remove_item(&p_subn->prtn_pkey_tbl,
					    (cl_map_item_t *) p);
			osm_prtn_delete(p_subn, &p);
		}
	}

	if (is_config && is_wrong_config) {
		OSM_LOG(p_log, OSM_LOG_ERROR, "Partition configuration "
			"in error; retrying with default config\n");
		is_config = FALSE;
		goto retry_default;
	}

_err:
	return status;
}