int osm_subn_parse_conf_file(const char *file_name, osm_subn_opt_t * p_opts)
{
	char line[1024];
	FILE *opts_file;
	char *p_key, *p_val, *pound_sign;
	const opt_rec_t *r;
	void *p_field1, *p_field2;
	int token_matched;

	opts_file = fopen(file_name, "r");
	if (!opts_file) {
		if (errno == ENOENT)
			return 1;
		printf("cannot open file \'%s\': %s\n",
		       file_name, strerror(errno));
		return -1;
	}

	printf(" Reading Cached Option File: %s\n", file_name);

	p_opts->config_file = file_name;
	if (!p_opts->file_opts && !(p_opts->file_opts = malloc(sizeof(*p_opts)))) {
		fclose(opts_file);
		return -1;
	}
	memcpy(p_opts->file_opts, p_opts, sizeof(*p_opts));
	p_opts->file_opts->file_opts = NULL;

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
			p_field1 = (void *)p_opts->file_opts + r->opt_offset;
			p_field2 = (void *)p_opts + r->opt_offset;
			/* don't call setup function first time */
			r->parse_fn(NULL, p_key, p_val, p_field1, p_field2,
				    NULL);
			break;
		}

		if (!token_matched)
			log_report(" Unrecognized token: \"%s\"\n", p_key);
	}
	fclose(opts_file);

	osm_subn_verify_config(p_opts);

	return 0;
}