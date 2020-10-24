static ib_api_status_t
osmtest_write_port_info(IN osmtest_t * const p_osmt,
			IN FILE * fh,
			IN const ib_portinfo_record_t * const p_rec)
{
	int result;
	cl_status_t status = IB_SUCCESS;

	OSM_LOG_ENTER(&p_osmt->log);

	result = fprintf(fh,
			 "DEFINE_PORT\n"
			 "lid                     0x%X\n"
			 "port_num                0x%X\n"
			 "m_key                   0x%016" PRIx64 "\n"
			 "subnet_prefix           0x%016" PRIx64 "\n"
			 "base_lid                0x%X\n"
			 "master_sm_base_lid      0x%X\n"
			 "capability_mask         0x%X\n"
			 "diag_code               0x%X\n"
			 "m_key_lease_period      0x%X\n"
			 "local_port_num          0x%X\n"
			 "link_width_enabled      0x%X\n"
			 "link_width_supported    0x%X\n"
			 "link_width_active       0x%X\n"
			 "link_speed_supported    0x%X\n"
			 "port_state              %s\n"
			 "state_info2             0x%X\n"
			 "mpb                     0x%X\n"
			 "lmc                     0x%X\n"
			 "link_speed              0x%X\n"
			 "mtu_smsl                0x%X\n"
			 "vl_cap                  0x%X\n"
			 "vl_high_limit           0x%X\n"
			 "vl_arb_high_cap         0x%X\n"
			 "vl_arb_low_cap          0x%X\n"
			 "mtu_cap                 0x%X\n"
			 "vl_stall_life           0x%X\n"
			 "vl_enforce              0x%X\n"
			 "m_key_violations        0x%X\n"
			 "p_key_violations        0x%X\n"
			 "q_key_violations        0x%X\n"
			 "guid_cap                0x%X\n"
			 "subnet_timeout          0x%X\n"
			 "resp_time_value         0x%X\n"
			 "error_threshold         0x%X\n"
			 "END\n\n",
			 cl_ntoh16(p_rec->lid),
			 p_rec->port_num,
			 cl_ntoh64(p_rec->port_info.m_key),
			 cl_ntoh64(p_rec->port_info.subnet_prefix),
			 cl_ntoh16(p_rec->port_info.base_lid),
			 cl_ntoh16(p_rec->port_info.master_sm_base_lid),
			 cl_ntoh32(p_rec->port_info.capability_mask),
			 cl_ntoh16(p_rec->port_info.diag_code),
			 cl_ntoh16(p_rec->port_info.m_key_lease_period),
			 p_rec->port_info.local_port_num,
			 p_rec->port_info.link_width_enabled,
			 p_rec->port_info.link_width_supported,
			 p_rec->port_info.link_width_active,
			 ib_port_info_get_link_speed_sup(&p_rec->port_info),
			 ib_get_port_state_str(ib_port_info_get_port_state
					       (&p_rec->port_info)),
			 p_rec->port_info.state_info2,
			 ib_port_info_get_mpb(&p_rec->port_info),
			 ib_port_info_get_lmc(&p_rec->port_info),
			 p_rec->port_info.link_speed, p_rec->port_info.mtu_smsl,
			 p_rec->port_info.vl_cap,
			 p_rec->port_info.vl_high_limit,
			 p_rec->port_info.vl_arb_high_cap,
			 p_rec->port_info.vl_arb_low_cap,
			 p_rec->port_info.mtu_cap,
			 p_rec->port_info.vl_stall_life,
			 p_rec->port_info.vl_enforce,
			 cl_ntoh16(p_rec->port_info.m_key_violations),
			 cl_ntoh16(p_rec->port_info.p_key_violations),
			 cl_ntoh16(p_rec->port_info.q_key_violations),
			 p_rec->port_info.guid_cap,
			 ib_port_info_get_timeout(&p_rec->port_info),
			 p_rec->port_info.resp_time_value,
			 p_rec->port_info.error_threshold);

	if (result < 0) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0161: "
			"Write failed\n");
		status = IB_ERROR;
		goto Exit;
	}

Exit:
	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}