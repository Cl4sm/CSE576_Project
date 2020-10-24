static ib_api_status_t
osmtest_parse_port(IN osmtest_t * const p_osmt,
		   IN FILE * const fh, IN OUT uint32_t * const p_line_num)
{
	ib_api_status_t status = IB_SUCCESS;
	uint32_t offset;
	char line[OSMTEST_MAX_LINE_LEN];
	boolean_t done = FALSE;
	port_t *p_port;
	const osmtest_token_t *p_tok;

	OSM_LOG_ENTER(&p_osmt->log);

	p_port = port_new();
	CL_ASSERT(p_port != NULL);

	/*
	 * Parse the inventory file and create the database.
	 */
	while (!done) {
		if (fgets(line, OSMTEST_MAX_LINE_LEN, fh) == NULL) {
			/*
			 * End of file in the middle of a definition.
			 */
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0123: "
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
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0124: "
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
			p_port->comp.lid = 0xFFFF;
			p_port->rec.lid =
			    cl_hton16((uint16_t)
				      strtoul(&line[offset], NULL, 0));
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG, "lid = 0x%X\n",
				cl_ntoh16(p_port->rec.lid));
			break;

		case OSMTEST_TOKEN_PORT_NUM:
			p_port->comp.port_num = 0xFF;
			p_port->rec.port_num =
			    (uint8_t) strtoul(&line[offset], NULL, 0);
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"port_num = 0x%u\n", p_port->rec.port_num);
			break;

		case OSMTEST_TOKEN_MKEY:
			p_port->comp.port_info.m_key = 0xFFFFFFFFFFFFFFFFULL;
			p_port->rec.port_info.m_key =
			    cl_hton64(strtoull(&line[offset], NULL, 0));
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"m_key = 0x%016" PRIx64 "\n",
				cl_ntoh64(p_port->rec.port_info.m_key));
			break;

		case OSMTEST_TOKEN_SUBN_PREF:
			p_port->comp.port_info.subnet_prefix =
			    0xFFFFFFFFFFFFFFFFULL;
			p_port->rec.port_info.subnet_prefix =
			    cl_hton64(strtoull(&line[offset], NULL, 0));
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"subnet_prefix = 0x%016" PRIx64 "\n",
				cl_ntoh64(p_port->rec.port_info.subnet_prefix));
			break;

		case OSMTEST_TOKEN_BASE_LID:
			p_port->comp.port_info.base_lid = 0xFFFF;
			p_port->rec.port_info.base_lid =
			    cl_hton16((uint16_t)
				      strtoul(&line[offset], NULL, 0));
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"base_lid = 0x%X\n",
				cl_ntoh16(p_port->rec.port_info.base_lid));
			break;

		case OSMTEST_TOKEN_SM_BASE_LID:
			p_port->comp.port_info.master_sm_base_lid = 0xFFFF;
			p_port->rec.port_info.master_sm_base_lid =
			    cl_hton16((uint16_t)
				      strtoul(&line[offset], NULL, 0));
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"master_sm_base_lid = 0x%X\n",
				cl_ntoh16(p_port->rec.port_info.master_sm_base_lid));
			break;

		case OSMTEST_TOKEN_CAP_MASK:
			p_port->comp.port_info.capability_mask = 0xFFFFFFFF;
			p_port->rec.port_info.capability_mask =
			    cl_hton32((uint32_t)
				      strtoul(&line[offset], NULL, 0));
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"capability_mask = 0x%X\n",
				cl_ntoh32(p_port->rec.port_info.capability_mask));
			break;

		case OSMTEST_TOKEN_DIAG_CODE:
			p_port->comp.port_info.diag_code = 0xFFFF;
			p_port->rec.port_info.diag_code =
			    cl_hton16((uint16_t)
				      strtoul(&line[offset], NULL, 0));
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"diag_code = 0x%X\n",
				cl_ntoh16(p_port->rec.port_info.diag_code));
			break;

		case OSMTEST_TOKEN_MKEY_LEASE_PER:
			p_port->comp.port_info.m_key_lease_period = 0xFFFF;
			p_port->rec.port_info.m_key_lease_period =
			    cl_hton16((uint16_t)
				      strtoul(&line[offset], NULL, 0));
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"m_key_lease_period = 0x%X\n",
				cl_ntoh16(p_port->rec.port_info.m_key_lease_period));
			break;

		case OSMTEST_TOKEN_LOC_PORT_NUM:
			p_port->comp.port_info.local_port_num = 0xFF;
			p_port->rec.port_info.local_port_num =
			    (uint8_t) strtoul(&line[offset], NULL, 0);
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"local_port_num = 0x%u\n",
				p_port->rec.port_info.local_port_num);
			break;

		case OSMTEST_TOKEN_LINK_WID_EN:
			p_port->comp.port_info.link_width_enabled = 0xFF;
			p_port->rec.port_info.link_width_enabled =
			    (uint8_t) strtoul(&line[offset], NULL, 0);
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"link_width_enabled = 0x%u\n",
				p_port->rec.port_info.link_width_enabled);
			break;

		case OSMTEST_TOKEN_LINK_WID_SUP:
			p_port->comp.port_info.link_width_supported = 0xFF;
			p_port->rec.port_info.link_width_supported =
			    (uint8_t) strtoul(&line[offset], NULL, 0);
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"link_width_supported = 0x%u\n",
				p_port->rec.port_info.link_width_supported);
			break;

		case OSMTEST_TOKEN_LINK_WID_ACT:
			p_port->comp.port_info.link_width_active = 0xFF;
			p_port->rec.port_info.link_width_active =
			    (uint8_t) strtoul(&line[offset], NULL, 0);
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"link_width_active = 0x%u\n",
				p_port->rec.port_info.link_width_active);
			break;

		case OSMTEST_TOKEN_LINK_SPEED_SUP:
			p_port->comp.port_info.state_info1 = 0xFF;
			ib_port_info_set_link_speed_sup((uint8_t)
							strtoul(&line[offset],
								NULL, 0),
							&p_port->rec.port_info);
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"link_speed_supported = 0x%u\n",
				ib_port_info_get_link_speed_sup(&p_port->rec.port_info));
			break;

		case OSMTEST_TOKEN_PORT_STATE:
			str_skip_white(line, &offset);
			p_port->comp.port_info.state_info1 = 0xFF;
			ib_port_info_set_port_state(&p_port->rec.port_info,
						    ib_get_port_state_from_str
						    (&line[offset]));
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"port_state = 0x%u\n",
				ib_port_info_get_port_state(&p_port->rec.port_info));
			break;

		case OSMTEST_TOKEN_STATE_INFO2:
			p_port->comp.port_info.state_info2 = 0xFF;
			p_port->rec.port_info.state_info2 =
			    (uint8_t) strtoul(&line[offset], NULL, 0);
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"state_info2 = 0x%u\n",
				p_port->rec.port_info.state_info2);
			break;

		case OSMTEST_TOKEN_MKEY_PROT_BITS:
			p_port->comp.port_info.mkey_lmc = 0xFF;
			ib_port_info_set_mpb(&p_port->rec.port_info,
					     (uint8_t) strtoul(&line[offset],
							       NULL, 0));
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG, "mpb = 0x%u\n",
				ib_port_info_get_mpb(&p_port->rec.port_info));
			break;

		case OSMTEST_TOKEN_LMC:
			p_port->comp.port_info.mkey_lmc = 0xFF;
			ib_port_info_set_lmc(&p_port->rec.port_info,
					     (uint8_t) strtoul(&line[offset],
							       NULL, 0));
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG, "lmc = 0x%u\n",
				ib_port_info_get_lmc(&p_port->rec.port_info));
			break;

		case OSMTEST_TOKEN_LINK_SPEED:
			p_port->comp.port_info.link_speed = 0xFF;
			p_port->rec.port_info.link_speed =
			    (uint8_t) strtoul(&line[offset], NULL, 0);
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"link_speed = 0x%u\n",
				p_port->rec.port_info.link_speed);
			break;

		case OSMTEST_TOKEN_MTU_SMSL:
			p_port->comp.port_info.mtu_smsl = 0xFF;
			p_port->rec.port_info.mtu_smsl =
			    (uint8_t) strtoul(&line[offset], NULL, 0);
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"mtu_smsl = 0x%u\n",
				p_port->rec.port_info.mtu_smsl);
			break;

		case OSMTEST_TOKEN_VL_CAP:
			p_port->comp.port_info.vl_cap = 0xFF;
			p_port->rec.port_info.vl_cap =
			    (uint8_t) strtoul(&line[offset], NULL, 0);
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG, "vl_cap = 0x%u\n",
				p_port->rec.port_info.vl_cap);
			break;

		case OSMTEST_TOKEN_VL_HIGH_LIMIT:
			p_port->comp.port_info.vl_high_limit = 0xFF;
			p_port->rec.port_info.vl_high_limit =
			    (uint8_t) strtoul(&line[offset], NULL, 0);
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"vl_high_limit = 0x%u\n",
				p_port->rec.port_info.vl_high_limit);
			break;

		case OSMTEST_TOKEN_VL_ARB_HIGH_CAP:
			p_port->comp.port_info.vl_arb_high_cap = 0xFF;
			p_port->rec.port_info.vl_arb_high_cap =
			    (uint8_t) strtoul(&line[offset], NULL, 0);
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"vl_arb_high_cap = 0x%u\n",
				p_port->rec.port_info.vl_arb_high_cap);
			break;

		case OSMTEST_TOKEN_VL_ARB_LOW_CAP:
			p_port->comp.port_info.vl_arb_low_cap = 0xFF;
			p_port->rec.port_info.vl_arb_low_cap =
			    (uint8_t) strtoul(&line[offset], NULL, 0);
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"vl_arb_low_cap = 0x%u\n",
				p_port->rec.port_info.vl_arb_low_cap);
			break;

		case OSMTEST_TOKEN_MTU_CAP:
			p_port->comp.port_info.mtu_cap = 0xFF;
			p_port->rec.port_info.mtu_cap =
			    (uint8_t) strtoul(&line[offset], NULL, 0);
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG, "mtu_cap = 0x%u\n",
				p_port->rec.port_info.mtu_cap);
			break;

		case OSMTEST_TOKEN_VL_STALL_LIFE:
			p_port->comp.port_info.vl_stall_life = 0xFF;
			p_port->rec.port_info.vl_stall_life =
			    (uint8_t) strtoul(&line[offset], NULL, 0);
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"vl_stall_life = 0x%u\n",
				p_port->rec.port_info.vl_stall_life);
			break;

		case OSMTEST_TOKEN_VL_ENFORCE:
			p_port->comp.port_info.vl_enforce = 0xFF;
			p_port->rec.port_info.vl_enforce =
			    (uint8_t) strtoul(&line[offset], NULL, 0);
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"vl_enforce = 0x%u\n",
				p_port->rec.port_info.vl_enforce);
			break;

		case OSMTEST_TOKEN_MKEY_VIOL:
			p_port->comp.port_info.m_key_violations = 0xFFFF;
			p_port->rec.port_info.m_key_violations =
			    cl_hton16((uint16_t)
				      strtoul(&line[offset], NULL, 0));
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"m_key_violations = 0x%X\n",
				cl_ntoh16(p_port->rec.port_info.m_key_violations));
			break;

		case OSMTEST_TOKEN_PKEY_VIOL:
			p_port->comp.port_info.p_key_violations = 0xFFFF;
			p_port->rec.port_info.p_key_violations =
			    cl_hton16((uint16_t)
				      strtoul(&line[offset], NULL, 0));
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"p_key_violations = 0x%X\n",
				cl_ntoh16(p_port->rec.port_info.p_key_violations));
			break;

		case OSMTEST_TOKEN_QKEY_VIOL:
			p_port->comp.port_info.q_key_violations = 0xFFFF;
			p_port->rec.port_info.q_key_violations =
			    cl_hton16((uint16_t)
				      strtoul(&line[offset], NULL, 0));
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"q_key_violations = 0x%X\n",
				cl_ntoh16(p_port->rec.port_info.q_key_violations));
			break;

		case OSMTEST_TOKEN_GUID_CAP:
			p_port->comp.port_info.guid_cap = 0xFF;
			p_port->rec.port_info.guid_cap =
			    (uint8_t) strtoul(&line[offset], NULL, 0);
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"guid_cap = 0x%u\n",
				p_port->rec.port_info.guid_cap);
			break;

		case OSMTEST_TOKEN_SUBN_TIMEOUT:
			p_port->comp.port_info.subnet_timeout = 0x1F;
			p_port->rec.port_info.subnet_timeout =
			    (uint8_t) strtoul(&line[offset], NULL, 0);
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"subnet_timeout = 0x%u\n",
				ib_port_info_get_timeout(&p_port->rec.port_info));
			break;

		case OSMTEST_TOKEN_RESP_TIME_VAL:
			p_port->comp.port_info.resp_time_value = 0xFF;
			p_port->rec.port_info.resp_time_value =
			    (uint8_t) strtoul(&line[offset], NULL, 0);
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"resp_time_value = 0x%u\n",
				p_port->rec.port_info.resp_time_value);
			break;

		case OSMTEST_TOKEN_ERR_THRESHOLD:
			p_port->comp.port_info.error_threshold = 0xFF;
			p_port->rec.port_info.error_threshold =
			    (uint8_t) strtoul(&line[offset], NULL, 0);
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
				"error_threshold = 0x%u\n",
				p_port->rec.port_info.error_threshold);
			break;

		case OSMTEST_TOKEN_END:
			done = TRUE;
			break;

		default:
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0125: "
				"Ignoring line %u with unknown token: %s\n",
				*p_line_num, &line[offset]);
			break;
		}
	}

	/*
	 * Make sure the user specified enough information, then
	 * add this object to the database.
	 */
	if (p_port->comp.lid == 0) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0126: "
			"LID must be specified for defined ports\n");
		port_delete(p_port);
		status = IB_ERROR;
		goto Exit;
	}

	cl_qmap_insert(&p_osmt->exp_subn.port_key_tbl,
		       port_gen_id(p_port->rec.lid, p_port->rec.port_num),
		       &p_port->map_item);

Exit:
	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}