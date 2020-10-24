int osm_subn_rescan_conf_files(IN osm_subn_t * p_subn)
{
	char line[1024];
	osm_subn_opt_t *p_opts = &p_subn->opt;
	const opt_rec_t *r;
	FILE *opts_file;
	char *p_key, *p_val, *pound_sign;
	void *p_field1, *p_field2;
	int token_matched;

	if (!p_opts->config_file)
		return 0;

	opts_file = fopen(p_opts->config_file, "r");
	if (!opts_file) {
		if (errno == ENOENT)
			return 1;
		OSM_LOG(&p_subn->p_osm->log, OSM_LOG_ERROR,
			"cannot open file \'%s\': %s\n",
			p_opts->config_file, strerror(errno));
		return -1;
	}

	subn_init_qos_options(&p_opts->qos_options,
			      &p_opts->file_opts->qos_options);
	subn_init_qos_options(&p_opts->qos_ca_options,
			      &p_opts->file_opts->qos_ca_options);
	subn_init_qos_options(&p_opts->qos_sw0_options,
			      &p_opts->file_opts->qos_sw0_options);
	subn_init_qos_options(&p_opts->qos_swe_options,
			      &p_opts->file_opts->qos_swe_options);
	subn_init_qos_options(&p_opts->qos_rtr_options,
			      &p_opts->file_opts->qos_rtr_options);

	while (fgets(line, 1023, opts_file) != NULL) {
		pound_sign = strchr(line,'#');
		token_matched = 0;

		/* Truncate any comments. */
		if (pound_sign)
			*pound_sign = '\0';

		/* get the first token */
		p_key = strtok_r(line, " \t\n", &p_val);
		if (!p_key)
			continue;

		p_val = clean_val(p_val);

		for (r = opt_tbl; r->name; r++) {
			if (strcmp(r->name, p_key))
				continue;

			token_matched = 1;

			if (!r->can_update)
				continue;

			p_field1 = (void *)p_opts->file_opts + r->opt_offset;
			p_field2 = (void *)p_opts + r->opt_offset;
			r->parse_fn(p_subn, p_key, p_val, p_field1, p_field2,
				    r->setup_fn);
			break;
		}
		if (!token_matched)
                       log_report(" Unrecognized token: \"%s\"\n", p_key);
	}
	fclose(opts_file);

	osm_subn_verify_config(p_opts);

	parse_prefix_routes_file(p_subn);

	parse_per_mod_logging_file(p_subn);

	return 0;
}