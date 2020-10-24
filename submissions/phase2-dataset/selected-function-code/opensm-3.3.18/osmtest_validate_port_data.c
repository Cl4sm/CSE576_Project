static ib_api_status_t
osmtest_validate_port_data(IN osmtest_t * const p_osmt,
			   IN port_t * const p_port,
			   IN const ib_portinfo_record_t * const p_rec)
{
	cl_status_t status = IB_SUCCESS;

	OSM_LOG_ENTER(&p_osmt->log);

	OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
		"Checking port LID 0x%X, Num 0x%X\n",
		cl_ntoh16(p_rec->lid), p_rec->port_num);

	/*
	 * Has this record already been returned?
	 */
	if (p_port->count != 0) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0036: "
			"Already received port LID 0x%X, Num 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num);
		status = IB_ERROR;
		goto Exit;
	}

	++p_port->count;

	/*
	 * Compare the fields the user wishes to validate.
	 */
	if ((p_port->comp.lid & p_port->rec.lid) !=
	    (p_port->comp.lid & p_rec->lid)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0037: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected LID 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			p_port->rec.lid, p_rec->lid);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_num & p_port->rec.port_num) !=
	    (p_port->comp.port_num & p_rec->port_num)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0038: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected port_num 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			p_port->rec.port_num, p_rec->port_num);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.m_key & p_port->rec.port_info.m_key) !=
	    (p_port->comp.port_info.m_key & p_rec->port_info.m_key)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0039: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected m_key 0x%016" PRIx64
			", received 0x%016" PRIx64 "\n", cl_ntoh16(p_rec->lid),
			p_rec->port_num, p_port->rec.port_info.m_key,
			p_rec->port_info.m_key);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.subnet_prefix & p_port->rec.port_info.
	     subnet_prefix) !=
	    (p_port->comp.port_info.subnet_prefix & p_rec->port_info.
	     subnet_prefix)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0040: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected subnet_prefix 0x%016" PRIx64
			", received 0x%016" PRIx64 "\n", cl_ntoh16(p_rec->lid),
			p_rec->port_num, p_port->rec.port_info.subnet_prefix,
			p_rec->port_info.subnet_prefix);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.base_lid & p_port->rec.port_info.
	     base_lid) !=
	    (p_port->comp.port_info.base_lid & p_rec->port_info.base_lid)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0041: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected base_lid 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			p_port->rec.port_info.base_lid,
			p_rec->port_info.base_lid);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.master_sm_base_lid & p_port->rec.port_info.
	     master_sm_base_lid) !=
	    (p_port->comp.port_info.master_sm_base_lid & p_rec->port_info.
	     master_sm_base_lid)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0042: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected master_sm_base_lid 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			p_port->rec.port_info.master_sm_base_lid,
			p_rec->port_info.master_sm_base_lid);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.capability_mask & p_port->rec.port_info.
	     capability_mask) !=
	    (p_port->comp.port_info.capability_mask & p_rec->port_info.
	     capability_mask)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0043: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected capability_mask 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			cl_ntoh32(p_port->rec.port_info.capability_mask),
			cl_ntoh32(p_rec->port_info.capability_mask));
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.diag_code & p_port->rec.port_info.
	     diag_code) !=
	    (p_port->comp.port_info.diag_code & p_rec->port_info.diag_code)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0044: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected diag_code 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			p_port->rec.port_info.diag_code,
			p_rec->port_info.diag_code);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.m_key_lease_period & p_port->rec.port_info.
	     m_key_lease_period) !=
	    (p_port->comp.port_info.m_key_lease_period & p_rec->port_info.
	     m_key_lease_period)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0045: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected m_key_lease_period 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			p_port->rec.port_info.m_key_lease_period,
			p_rec->port_info.m_key_lease_period);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.local_port_num & p_port->rec.port_info.
	     local_port_num) !=
	    (p_port->comp.port_info.local_port_num & p_rec->port_info.
	     local_port_num)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0046: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected local_port_num 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			p_port->rec.port_info.local_port_num,
			p_rec->port_info.local_port_num);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.link_width_enabled & p_port->rec.port_info.
	     link_width_enabled) !=
	    (p_port->comp.port_info.link_width_enabled & p_rec->port_info.
	     link_width_enabled)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0047: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected link_width_enabled 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			p_port->rec.port_info.link_width_enabled,
			p_rec->port_info.link_width_enabled);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.link_width_supported & p_port->rec.
	     port_info.link_width_supported) !=
	    (p_port->comp.port_info.link_width_supported & p_rec->port_info.
	     link_width_supported)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0048: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected link_width_supported 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			p_port->rec.port_info.link_width_supported,
			p_rec->port_info.link_width_supported);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.link_width_active & p_port->rec.port_info.
	     link_width_active) !=
	    (p_port->comp.port_info.link_width_active & p_rec->port_info.
	     link_width_active)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0049: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected link_width_active 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			p_port->rec.port_info.link_width_active,
			p_rec->port_info.link_width_active);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.link_speed & p_port->rec.port_info.
	     link_speed) !=
	    (p_port->comp.port_info.link_speed & p_rec->port_info.link_speed)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0054: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected link_speed 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			p_port->rec.port_info.link_speed,
			p_rec->port_info.link_speed);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.state_info1 & p_port->rec.port_info.
	     state_info1) !=
	    (p_port->comp.port_info.state_info1 & p_rec->port_info.
	     state_info1)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0055: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected state_info1 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			p_port->rec.port_info.state_info1,
			p_rec->port_info.state_info1);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.state_info2 & p_port->rec.port_info.
	     state_info2) !=
	    (p_port->comp.port_info.state_info2 & p_rec->port_info.
	     state_info2)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0058: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected state_info2 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			p_port->rec.port_info.state_info2,
			p_rec->port_info.state_info2);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.mkey_lmc & p_port->rec.port_info.
	     mkey_lmc) !=
	    (p_port->comp.port_info.mkey_lmc & p_rec->port_info.mkey_lmc)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0059: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected mkey_lmc 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			p_port->rec.port_info.mkey_lmc,
			p_rec->port_info.mkey_lmc);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.link_speed & p_port->rec.port_info.
	     link_speed) !=
	    (p_port->comp.port_info.link_speed & p_rec->port_info.link_speed)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0060: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected link_speed 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			p_port->rec.port_info.link_speed,
			p_rec->port_info.link_speed);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.mtu_smsl & p_port->rec.port_info.
	     mtu_smsl) !=
	    (p_port->comp.port_info.mtu_smsl & p_rec->port_info.mtu_smsl)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0061: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected mtu_smsl 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			p_port->rec.port_info.mtu_smsl,
			p_rec->port_info.mtu_smsl);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.vl_cap & p_port->rec.port_info.vl_cap) !=
	    (p_port->comp.port_info.vl_cap & p_rec->port_info.vl_cap)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0062: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected vl_cap 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			p_port->rec.port_info.vl_cap, p_rec->port_info.vl_cap);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.vl_high_limit & p_port->rec.port_info.
	     vl_high_limit) !=
	    (p_port->comp.port_info.vl_high_limit & p_rec->port_info.
	     vl_high_limit)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0082: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected vl_high_limit 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			p_port->rec.port_info.vl_high_limit,
			p_rec->port_info.vl_high_limit);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.vl_arb_high_cap & p_port->rec.port_info.
	     vl_arb_high_cap) !=
	    (p_port->comp.port_info.vl_arb_high_cap & p_rec->port_info.
	     vl_arb_high_cap)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0083: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected vl_arb_high_cap 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			p_port->rec.port_info.vl_arb_high_cap,
			p_rec->port_info.vl_arb_high_cap);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.vl_arb_low_cap & p_port->rec.port_info.
	     vl_arb_low_cap) !=
	    (p_port->comp.port_info.vl_arb_low_cap & p_rec->port_info.
	     vl_arb_low_cap)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0084: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected vl_arb_low_cap 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			p_port->rec.port_info.vl_arb_low_cap,
			p_rec->port_info.vl_arb_low_cap);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.mtu_cap & p_port->rec.port_info.mtu_cap) !=
	    (p_port->comp.port_info.mtu_cap & p_rec->port_info.mtu_cap)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0085: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected mtu_cap 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			p_port->rec.port_info.mtu_cap,
			p_rec->port_info.mtu_cap);
		status = IB_ERROR;
		goto Exit;
	}
#if 0
	/* this is a dynamic attribute */
	if ((p_port->comp.port_info.vl_stall_life & p_port->rec.port_info.
	     vl_stall_life) !=
	    (p_port->comp.port_info.vl_stall_life & p_rec->port_info.
	     vl_stall_life)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 012F: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected vl_stall_life 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			p_port->rec.port_info.vl_stall_life,
			p_rec->port_info.vl_stall_life);
		status = IB_ERROR;
		goto Exit;
	}
#endif

	if ((p_port->comp.port_info.vl_enforce & p_port->rec.port_info.
	     vl_enforce) !=
	    (p_port->comp.port_info.vl_enforce & p_rec->port_info.vl_enforce)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0086: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected vl_enforce 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			p_port->rec.port_info.vl_enforce,
			p_rec->port_info.vl_enforce);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.m_key_violations & p_port->rec.port_info.
	     m_key_violations) !=
	    (p_port->comp.port_info.m_key_violations & p_rec->port_info.
	     m_key_violations)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0087: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected m_key_violations 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			cl_ntoh16(p_port->rec.port_info.m_key_violations),
			cl_ntoh16(p_rec->port_info.m_key_violations));
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.p_key_violations & p_port->rec.port_info.
	     p_key_violations) !=
	    (p_port->comp.port_info.p_key_violations & p_rec->port_info.
	     p_key_violations)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0088: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected p_key_violations 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			cl_ntoh16(p_port->rec.port_info.p_key_violations),
			cl_ntoh16(p_rec->port_info.p_key_violations));
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.q_key_violations & p_port->rec.port_info.
	     q_key_violations) !=
	    (p_port->comp.port_info.q_key_violations & p_rec->port_info.
	     q_key_violations)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0089: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected q_key_violations 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			cl_ntoh16(p_port->rec.port_info.q_key_violations),
			cl_ntoh16(p_rec->port_info.q_key_violations));
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.guid_cap & p_port->rec.port_info.
	     guid_cap) !=
	    (p_port->comp.port_info.guid_cap & p_rec->port_info.guid_cap)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0090: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected guid_cap 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			p_port->rec.port_info.guid_cap,
			p_rec->port_info.guid_cap);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.subnet_timeout & p_port->rec.port_info.
	     subnet_timeout) !=
	    (p_port->comp.port_info.subnet_timeout & p_rec->port_info.
	     subnet_timeout)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0091: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected subnet_timeout 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			ib_port_info_get_timeout(&p_port->rec.port_info),
			ib_port_info_get_timeout(&p_rec->port_info));
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.resp_time_value & p_port->rec.port_info.
	     resp_time_value) !=
	    (p_port->comp.port_info.resp_time_value & p_rec->port_info.
	     resp_time_value)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0092: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected resp_time_value 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			p_port->rec.port_info.resp_time_value,
			p_rec->port_info.resp_time_value);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_port->comp.port_info.error_threshold & p_port->rec.port_info.
	     error_threshold) !=
	    (p_port->comp.port_info.error_threshold & p_rec->port_info.
	     error_threshold)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0093: "
			"Field mismatch port LID 0x%X Num:0x%X\n"
			"\t\t\t\tExpected error_threshold 0x%X, received 0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num,
			p_port->rec.port_info.error_threshold,
			p_rec->port_info.error_threshold);
		status = IB_ERROR;
		goto Exit;
	}

Exit:
	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}