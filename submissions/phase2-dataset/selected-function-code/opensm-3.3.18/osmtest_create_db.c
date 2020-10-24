static ib_api_status_t osmtest_create_db(IN osmtest_t * const p_osmt)
{
	FILE *fh;
	ib_api_status_t status = IB_SUCCESS;
	uint32_t offset;
	char line[OSMTEST_MAX_LINE_LEN];
	uint32_t line_num = 0;
	const osmtest_token_t *p_tok;
	boolean_t got_error = FALSE;

	OSM_LOG_ENTER(&p_osmt->log);

	fh = fopen(p_osmt->opt.file_name, "r");
	if (fh == NULL) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0130: "
			"Unable to open inventory file (%s)\n",
			p_osmt->opt.file_name);
		status = IB_ERROR;
		goto Exit;
	}

	/*
	 * Parse the inventory file and create the database.
	 */
	while (fgets(line, OSMTEST_MAX_LINE_LEN, fh) != NULL) {
		line_num++;

		/*
		 * Skip whitespace
		 */
		offset = 0;
		if (!str_skip_white(line, &offset))
			continue;	/* whole line was whitespace */

		p_tok = str_get_token(&line[offset]);
		if (p_tok == NULL) {
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0131: "
				"Ignoring line %u: %s\n", line_num,
				&line[offset]);
			got_error = TRUE;
			continue;
		}

		OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
			"Found '%s' (line %u)\n", p_tok->str, line_num);

		switch (p_tok->val) {
		case OSMTEST_TOKEN_COMMENT:
			break;

		case OSMTEST_TOKEN_DEFINE_NODE:
			status = osmtest_parse_node(p_osmt, fh, &line_num);
			break;

		case OSMTEST_TOKEN_DEFINE_PORT:
			status = osmtest_parse_port(p_osmt, fh, &line_num);
			break;

		case OSMTEST_TOKEN_DEFINE_PATH:
			status = osmtest_parse_path(p_osmt, fh, &line_num);
			break;

		case OSMTEST_TOKEN_DEFINE_LINK:
			status = osmtest_parse_link(p_osmt, fh, &line_num);
			break;

		default:
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0132: "
				"Ignoring line %u: %s\n", line_num,
				&line[offset]);
			got_error = TRUE;
			break;
		}

		if (got_error)
			status = IB_ERROR;

		if (status != IB_SUCCESS) {
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0133: "
				"Bad status received during parsing (%s)\n",
				ib_get_err_str(status));
			fclose(fh);
			goto Exit;
		}
	}

	fclose(fh);

Exit:
	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}