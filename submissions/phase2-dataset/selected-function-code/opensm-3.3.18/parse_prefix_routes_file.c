static ib_api_status_t parse_prefix_routes_file(IN osm_subn_t * p_subn)
{
	osm_log_t *log = &p_subn->p_osm->log;
	FILE *fp;
	char buf[1024];
	int line = 0;
	int errors = 0;

	while (!cl_is_qlist_empty(&p_subn->prefix_routes_list)) {
		cl_list_item_t *item = cl_qlist_remove_head(&p_subn->prefix_routes_list);
		free(item);
	}

	fp = fopen(p_subn->opt.prefix_routes_file, "r");
	if (! fp) {
		if (errno == ENOENT)
			return IB_SUCCESS;

		OSM_LOG(log, OSM_LOG_ERROR, "fopen(%s) failed: %s",
			p_subn->opt.prefix_routes_file, strerror(errno));
		return IB_ERROR;
	}

	while (fgets(buf, sizeof buf, fp) != NULL) {
		char *p_prefix, *p_guid, *p_extra, *p_last, *p_end;
		uint64_t prefix, guid;

		line++;
		if (errors > 10)
			break;

		p_prefix = strtok_r(buf, " \t\n", &p_last);
		if (! p_prefix)
			continue; /* ignore blank lines */

		if (*p_prefix == '#')
			continue; /* ignore comment lines */

		p_guid = strtok_r(NULL, " \t\n", &p_last);
		if (! p_guid) {
			OSM_LOG(log, OSM_LOG_ERROR, "%s:%d: missing GUID\n",
				p_subn->opt.prefix_routes_file, line);
			errors++;
			continue;
		}

		p_extra = strtok_r(NULL, " \t\n", &p_last);
		if (p_extra && *p_extra != '#') {
			OSM_LOG(log, OSM_LOG_INFO, "%s:%d: extra tokens ignored\n",
				p_subn->opt.prefix_routes_file, line);
		}

		if (strcmp(p_prefix, "*") == 0)
			prefix = 0;
		else {
			prefix = strtoull(p_prefix, &p_end, 16);
			if (*p_end != '\0') {
				OSM_LOG(log, OSM_LOG_ERROR, "%s:%d: illegal prefix: %s\n",
					p_subn->opt.prefix_routes_file, line, p_prefix);
				errors++;
				continue;
			}
		}

		if (strcmp(p_guid, "*") == 0)
			guid = 0;
		else {
			guid = strtoull(p_guid, &p_end, 16);
			if (*p_end != '\0' && *p_end != '#') {
				OSM_LOG(log, OSM_LOG_ERROR, "%s:%d: illegal GUID: %s\n",
					p_subn->opt.prefix_routes_file, line, p_guid);
				errors++;
				continue;
			}
		}

		if (append_prefix_route(p_subn, prefix, guid) != IB_SUCCESS) {
			errors++;
			break;
		}
	}

	fclose(fp);
	return (errors == 0) ? IB_SUCCESS : IB_ERROR;
}