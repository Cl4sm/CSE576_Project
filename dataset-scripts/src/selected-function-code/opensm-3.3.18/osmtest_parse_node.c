static ib_api_status_t
osmtest_parse_node(IN osmtest_t * const p_osmt,
		   IN FILE * const fh, IN OUT uint32_t * const p_line_num)
{
	ib_api_status_t status = IB_SUCCESS;
	uint32_t offset;
	char line[OSMTEST_MAX_LINE_LEN];
	boolean_t done = FALSE;
	node_t *p_node;
	node_t *p_guid_node;
	const osmtest_token_t *p_tok;

	OSM_LOG_ENTER(&p_osmt->log);

	p_node = node_new();
	CL_ASSERT(p_node != NULL);

	/*
	 * Parse the inventory file and create the database.
	 */
	while (!done) {
		if (fgets(line, OSMTEST_MAX_LINE_LEN, fh) == NULL) {
			/*
			 * End of file in the middle of a definition.
			 */
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0119: "
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
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0120: "
				"Ignoring line %u with unknown token: %s\n",
				*p_line_num, &line[offset]);
			continue;
		}

		OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
			"Found '%s' (line %u)\n", p_tok->str, *p_line_num);

		str_skip_token(line, &offset);

		switch (p_tok->val) {
		case OSMTEST_TOKEN_COMMENT:
			break;

		case OSMTEST_TOKEN_LID:
			p_node->comp.lid = 0xFFFF;
			p_node->rec.lid =
			    cl_hton16((uint16_t)
				      strtoul(&line[offset], NULL, 0));
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG, "lid = 0x%X\n",
				cl_ntoh16(p_node->rec.lid));
			break;

		case OSMTEST_TOKEN_BASE_VERSION:
			p_node->comp.node_info.base_version = 0xFF;
			p_node->rec.node_info.base_version =
			    (uint8_t) strtoul(&line[offset], NULL, 0);
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"base_version = 0x%X\n",
				p_node->rec.node_info.base_version);
			break;

		case OSMTEST_TOKEN_CLASS_VERSION:
			p_node->comp.node_info.class_version = 0xFF;
			p_node->rec.node_info.class_version =
			    (uint8_t) strtoul(&line[offset], NULL, 0);
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"class_version = 0x%X\n",
				p_node->rec.node_info.class_version);
			break;

		case OSMTEST_TOKEN_NODE_TYPE:
			p_node->comp.node_info.node_type = 0xFF;
			p_node->rec.node_info.node_type =
			    (uint8_t) strtoul(&line[offset], NULL, 0);
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"node_type = 0x%X\n",
				p_node->rec.node_info.node_type);
			break;

		case OSMTEST_TOKEN_NUM_PORTS:
			p_node->comp.node_info.num_ports = 0xFF;
			p_node->rec.node_info.num_ports =
			    (uint8_t) strtoul(&line[offset], NULL, 0);
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"num_ports = 0x%X\n",
				p_node->rec.node_info.num_ports);
			break;

		case OSMTEST_TOKEN_SYS_GUID:
			p_node->comp.node_info.sys_guid = 0xFFFFFFFFFFFFFFFFULL;
			p_node->rec.node_info.sys_guid =
			    cl_hton64(strtoull(&line[offset], NULL, 0));
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"sys_guid = 0x%016" PRIx64 "\n",
				cl_ntoh64(p_node->rec.node_info.sys_guid));
			break;

		case OSMTEST_TOKEN_NODE_GUID:
			p_node->comp.node_info.node_guid =
			    0xFFFFFFFFFFFFFFFFULL;
			p_node->rec.node_info.node_guid =
			    cl_hton64(strtoull(&line[offset], NULL, 0));
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"node_guid = 0x%016" PRIx64 "\n",
				cl_ntoh64(p_node->rec.node_info.node_guid));
			break;

		case OSMTEST_TOKEN_PORT_GUID:
			p_node->comp.node_info.port_guid =
			    0xFFFFFFFFFFFFFFFFULL;
			p_node->rec.node_info.port_guid =
			    cl_hton64(strtoull(&line[offset], NULL, 0));
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"port_guid = 0x%016" PRIx64 "\n",
				cl_ntoh64(p_node->rec.node_info.port_guid));
			break;

		case OSMTEST_TOKEN_PARTITION_CAP:
			p_node->comp.node_info.partition_cap = 0xFFFF;
			p_node->rec.node_info.partition_cap =
			    cl_hton16((uint16_t)
				      strtoul(&line[offset], NULL, 0));
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"partition_cap = 0x%X\n",
				cl_ntoh16(p_node->rec.node_info.partition_cap));
			break;

		case OSMTEST_TOKEN_DEVICE_ID:
			p_node->comp.node_info.device_id = 0xFFFF;
			p_node->rec.node_info.device_id = cl_hton16((uint16_t)
								    strtoul
								    (&line
								     [offset],
								     NULL, 0));
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"device_id = 0x%X\n",
				cl_ntoh16(p_node->rec.node_info.device_id));
			break;

		case OSMTEST_TOKEN_REVISION:
			p_node->comp.node_info.revision = 0xFFFFFFFF;
			p_node->rec.node_info.revision =
			    cl_hton32(strtoul(&line[offset], NULL, 0));
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"revision = 0x%X\n",
				cl_ntoh32(p_node->rec.node_info.revision));
			break;

		case OSMTEST_TOKEN_PORT_NUM:
			p_node->comp.node_info.port_num_vendor_id |=
			    IB_NODE_INFO_PORT_NUM_MASK;
			p_node->rec.node_info.port_num_vendor_id |=
			    (uint8_t) strtoul(&line[offset], NULL, 0);
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"local_port_num = 0x%X\n",
				ib_node_info_get_local_port_num
				(&p_node->rec.node_info));
			break;

		case OSMTEST_TOKEN_VENDOR_ID:
			p_node->comp.node_info.port_num_vendor_id |=
			    IB_NODE_INFO_VEND_ID_MASK;
			p_node->rec.node_info.port_num_vendor_id |=
			    cl_hton32(strtoul(&line[offset], NULL, 0));
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"vendor_id = 0x%X\n",
				cl_ntoh32(ib_node_info_get_vendor_id
					  (&p_node->rec.node_info)));
			break;

		case OSMTEST_TOKEN_END:
			done = TRUE;
			break;

		default:
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0121: "
				"Ignoring line %u with unknown token: %s\n",
				*p_line_num, &line[offset]);

			break;
		}
	}

	/*
	 * Make sure the user specified enough information, then
	 * add this object to the database.
	 */
	if (p_node->comp.lid == 0) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0122: "
			"LID must be specified for defined nodes\n");
		node_delete(p_node);
		goto Exit;
	}

	cl_qmap_insert(&p_osmt->exp_subn.node_lid_tbl,
		       p_node->rec.lid, &p_node->map_item);

	p_guid_node = node_new();
	CL_ASSERT(p_node != NULL);

	*p_guid_node = *p_node;

	cl_qmap_insert(&p_osmt->exp_subn.node_guid_tbl,
		       p_guid_node->rec.node_info.node_guid,
		       &p_guid_node->map_item);

Exit:
	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}