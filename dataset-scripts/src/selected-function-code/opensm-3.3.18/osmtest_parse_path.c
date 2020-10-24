static ib_api_status_t
osmtest_parse_path(IN osmtest_t * const p_osmt,
		   IN FILE * const fh, IN OUT uint32_t * const p_line_num)
{
	ib_api_status_t status = IB_SUCCESS;
	uint32_t offset;
	char line[OSMTEST_MAX_LINE_LEN];
	boolean_t done = FALSE;
	path_t *p_path;
	const osmtest_token_t *p_tok;
	boolean_t got_error = FALSE;

	OSM_LOG_ENTER(&p_osmt->log);

	p_path = path_new();
	CL_ASSERT(p_path != NULL);

	/*
	 * Parse the inventory file and create the database.
	 */
	while (!done) {
		if (fgets(line, OSMTEST_MAX_LINE_LEN, fh) == NULL) {
			/*
			 * End of file in the middle of a definition.
			 */
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0127: "
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
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0128: "
				"Ignoring line %u with unknown token: %s\n",
				*p_line_num, &line[offset]);
			got_error = TRUE;
			continue;
		}

		OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
			"Found '%s' (line %u)\n", p_tok->str, *p_line_num);

		str_skip_token(line, &offset);

		switch (p_tok->val) {
		case OSMTEST_TOKEN_COMMENT:
			break;

		case OSMTEST_TOKEN_DGID:
			p_path->comp.dgid.unicast.prefix =
			    0xFFFFFFFFFFFFFFFFULL;
			p_path->comp.dgid.unicast.interface_id =
			    0xFFFFFFFFFFFFFFFFULL;

			str_skip_white(line, &offset);
			p_path->rec.dgid.unicast.prefix =
			    cl_hton64(strtoull(&line[offset], NULL, 0));
			str_skip_token(line, &offset);
			p_path->rec.dgid.unicast.interface_id =
			    cl_hton64(strtoull(&line[offset], NULL, 0));

			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"dgid = 0x%016" PRIx64 " 0x%016" PRIx64 "\n",
				cl_ntoh64(p_path->rec.dgid.unicast.prefix),
				cl_ntoh64(p_path->rec.dgid.unicast.interface_id));
			break;

		case OSMTEST_TOKEN_SGID:
			p_path->comp.sgid.unicast.prefix =
			    0xFFFFFFFFFFFFFFFFULL;
			p_path->comp.sgid.unicast.interface_id =
			    0xFFFFFFFFFFFFFFFFULL;

			str_skip_white(line, &offset);
			p_path->rec.sgid.unicast.prefix =
			    cl_hton64(strtoull(&line[offset], NULL, 0));
			str_skip_token(line, &offset);
			p_path->rec.sgid.unicast.interface_id =
			    cl_hton64(strtoull(&line[offset], NULL, 0));

			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"sgid = 0x%016" PRIx64 " 0x%016" PRIx64 "\n",
				cl_ntoh64(p_path->rec.sgid.unicast.prefix),
				cl_ntoh64(p_path->rec.sgid.unicast.interface_id));
			break;

		case OSMTEST_TOKEN_DLID:
			p_path->comp.dlid = 0xFFFF;
			p_path->rec.dlid =
			    cl_hton16((uint16_t)
				      strtoul(&line[offset], NULL, 0));
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG, "dlid = 0x%X\n",
				cl_ntoh16(p_path->rec.dlid));
			break;

		case OSMTEST_TOKEN_SLID:
			p_path->comp.slid = 0xFFFF;
			p_path->rec.slid =
			    cl_hton16((uint16_t)
				      strtoul(&line[offset], NULL, 0));
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG, "slid = 0x%X\n",
				cl_ntoh16(p_path->rec.slid));
			break;

		case OSMTEST_TOKEN_PKEY:
			p_path->comp.pkey = 0xFFFF;
			p_path->rec.pkey =
			    cl_hton16((uint16_t)
				      strtoul(&line[offset], NULL, 0));
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG, "pkey = 0x%X\n",
				cl_ntoh16(p_path->rec.pkey));
			break;

		case OSMTEST_TOKEN_END:
			done = TRUE;
			break;

		default:
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0129: "
				"Ignoring line %u with unknown token: %s\n",
				*p_line_num, &line[offset]);
			got_error = TRUE;
			break;
		}
	}

	if (got_error) {
		status = IB_ERROR;
		goto Exit;
	}
	/*
	 * Make sure the user specified enough information, then
	 * add this object to the database.
	 */
	if (osmtest_path_rec_kay_is_valid(p_osmt, p_path) == FALSE) {
		path_delete(p_path);
		status = IB_ERROR;
		goto Exit;
	}

	cl_qmap_insert(&p_osmt->exp_subn.path_tbl,
		       osmtest_path_rec_key_get(&p_path->rec),
		       &p_path->map_item);

Exit:
	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}