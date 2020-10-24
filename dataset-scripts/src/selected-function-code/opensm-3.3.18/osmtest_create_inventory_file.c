static ib_api_status_t
osmtest_create_inventory_file(IN osmtest_t * const p_osmt)
{
	FILE *fh;
	ib_api_status_t status = IB_SUCCESS;

	OSM_LOG_ENTER(&p_osmt->log);

	fh = fopen(p_osmt->opt.file_name, "w");
	if (fh == NULL) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0079: "
			"Unable to open inventory file (%s)\n",
			p_osmt->opt.file_name);
		status = IB_ERROR;
		goto Exit;
	}

	/* HACK: the order is important: nodes ports paths */
	status = osmtest_write_all_node_recs(p_osmt, fh);
	if (status != IB_SUCCESS)
		goto Exit;

	status = osmtest_write_all_port_recs(p_osmt, fh);
	if (status != IB_SUCCESS)
		goto Exit;

	if (!p_osmt->opt.ignore_path_records) {
		status = osmtest_write_all_path_recs(p_osmt, fh);
		if (status != IB_SUCCESS)
			goto Exit;
	}

	status = osmtest_write_all_link_recs(p_osmt, fh);
	if (status != IB_SUCCESS)
		goto Exit;

	fclose(fh);

Exit:
	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}