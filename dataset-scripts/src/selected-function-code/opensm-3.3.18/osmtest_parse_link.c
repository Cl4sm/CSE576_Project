static ib_api_status_t
osmtest_parse_link(IN osmtest_t * const p_osmt,
		   IN FILE * const fh, IN OUT uint32_t * const p_line_num)
{
	ib_api_status_t status = IB_SUCCESS;
	uint32_t offset;
	char line[OSMTEST_MAX_LINE_LEN];
	boolean_t done = FALSE;
	const osmtest_token_t *p_tok;
	boolean_t got_error = FALSE;

	OSM_LOG_ENTER(&p_osmt->log);

	/*
	 * Parse the inventory file and create the database.
	 */
	while (!done) {
		if (fgets(line, OSMTEST_MAX_LINE_LEN, fh) == NULL) {
			/*
			 * End of file in the middle of a definition.
			 */
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 012A: "
				"Unexpected end of file\n");
			status = IB_ERROR;
			goto Exit;
		}

		++*p_line_num;

		/*
		 * Skip whitespace
		 */
		offset = 0;
		if (!str_skip_white(line, &offset))
			continue;	/* whole line was whitespace */

		p_tok = str_get_token(&line[offset]);
		if (p_tok == NULL) {
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 012B: "
				"Ignoring line %u with unknown token: %s\n",
				*p_line_num, &line[offset]);
			got_error = TRUE;
			continue;
		}

		OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
			"Found '%s' (line %u)\n", p_tok->str, *p_line_num);

		str_skip_token(line, &offset);

		switch (p_tok->val) {
		case OSMTEST_TOKEN_FROMLID:
		case OSMTEST_TOKEN_FROMPORTNUM:
		case OSMTEST_TOKEN_TOPORTNUM:
		case OSMTEST_TOKEN_TOLID:
			/* For now */
			break;

		case OSMTEST_TOKEN_END:
			done = TRUE;
			break;

		default:
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 012C: "
				"Ignoring line %u with unknown token: %s\n",
				*p_line_num, &line[offset]);
			got_error = TRUE;
			break;
		}
	}

	if (got_error)
		status = IB_ERROR;

Exit:
	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}