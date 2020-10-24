static ib_api_status_t parse_per_mod_logging_file(IN osm_subn_t * p_subn)
{
	osm_log_t *log = &p_subn->p_osm->log;
	FILE *fp;
	char buf[1024];
	int line = 0;
	int errors = 0;

	osm_reset_log_per_module(log);

	if (p_subn->opt.per_module_logging_file == NULL)
		return IB_SUCCESS;

	fp = fopen(p_subn->opt.per_module_logging_file, "r");
	if (!fp) {
		if (errno == ENOENT)
			return IB_SUCCESS;

		OSM_LOG(log, OSM_LOG_ERROR, "fopen(%s) failed: %s",
			p_subn->opt.per_module_logging_file, strerror(errno));
		return IB_ERROR;
	}

	while (fgets(buf, sizeof buf, fp) != NULL) {
		char *p_mod_name, *p_level, *p_extra, *p_last;
		osm_log_level_t level;

		line++;
		if (errors > 10)
			break;

		p_mod_name = strtok_r(buf, " =,\t\n", &p_last);
		if (!p_mod_name)
			continue; /* ignore blank lines */

		if (*p_mod_name == '#')
			continue; /* ignore comment lines */

		p_level = strtok_r(NULL, " \t\n", &p_last);
		if (!p_level) {
			OSM_LOG(log, OSM_LOG_ERROR, "%s:%d: missing log level\n",
				p_subn->opt.per_module_logging_file, line);
			errors++;
			continue;
		}
		p_extra = strtok_r(NULL, " \t\n", &p_last);
		if (p_extra && *p_extra != '#') {
			OSM_LOG(log, OSM_LOG_INFO, "%s:%d: extra tokens ignored\n",
				p_subn->opt.per_module_logging_file, line);
		}

		level = strtoul(p_level, NULL, 0);
		if (insert_per_module_debug(p_subn, p_mod_name, level) != IB_SUCCESS) {
			errors++;
			break;
		}
	}

	fclose(fp);
	return (errors == 0) ? IB_SUCCESS : IB_ERROR;
}