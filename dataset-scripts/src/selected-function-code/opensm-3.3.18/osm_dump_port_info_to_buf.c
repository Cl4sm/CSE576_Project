static void osm_dump_port_info_to_buf(IN ib_net64_t node_guid,
				      IN ib_net64_t port_guid,
				      IN uint8_t port_num,
				      IN const ib_port_info_t * p_pi,
				      OUT char * buf)
{
	if (!buf || !p_pi)
		return;
	else {
		sprintf(buf,
			"PortInfo dump:\n"
			"\t\t\t\tport number..............%u\n"
			"\t\t\t\tnode_guid................0x%016" PRIx64 "\n"
			"\t\t\t\tport_guid................0x%016" PRIx64 "\n"
			"\t\t\t\tm_key....................0x%016" PRIx64 "\n"
			"\t\t\t\tsubnet_prefix............0x%016" PRIx64 "\n"
			"\t\t\t\tbase_lid.................%u\n"
			"\t\t\t\tmaster_sm_base_lid.......%u\n"
			"\t\t\t\tcapability_mask..........0x%X\n"
			"\t\t\t\tdiag_code................0x%X\n"
			"\t\t\t\tm_key_lease_period.......0x%X\n"
			"\t\t\t\tlocal_port_num...........%u\n"
			"\t\t\t\tlink_width_enabled.......0x%X\n"
			"\t\t\t\tlink_width_supported.....0x%X\n"
			"\t\t\t\tlink_width_active........0x%X\n"
			"\t\t\t\tlink_speed_supported.....0x%X\n"
			"\t\t\t\tport_state...............%s\n"
			"\t\t\t\tstate_info2..............0x%X\n"
			"\t\t\t\tm_key_protect_bits.......0x%X\n"
			"\t\t\t\tlmc......................0x%X\n"
			"\t\t\t\tlink_speed...............0x%X\n"
			"\t\t\t\tmtu_smsl.................0x%X\n"
			"\t\t\t\tvl_cap_init_type.........0x%X\n"
			"\t\t\t\tvl_high_limit............0x%X\n"
			"\t\t\t\tvl_arb_high_cap..........0x%X\n"
			"\t\t\t\tvl_arb_low_cap...........0x%X\n"
			"\t\t\t\tinit_rep_mtu_cap.........0x%X\n"
			"\t\t\t\tvl_stall_life............0x%X\n"
			"\t\t\t\tvl_enforce...............0x%X\n"
			"\t\t\t\tm_key_violations.........0x%X\n"
			"\t\t\t\tp_key_violations.........0x%X\n"
			"\t\t\t\tq_key_violations.........0x%X\n"
			"\t\t\t\tguid_cap.................0x%X\n"
			"\t\t\t\tclient_reregister........0x%X\n"
			"\t\t\t\tmcast_pkey_trap_suppr....0x%X\n"
			"\t\t\t\tsubnet_timeout...........0x%X\n"
			"\t\t\t\tresp_time_value..........0x%X\n"
			"\t\t\t\terror_threshold..........0x%X\n"
			"\t\t\t\tmax_credit_hint..........0x%X\n"
			"\t\t\t\tlink_round_trip_latency..0x%X\n"
			"\t\t\t\tcapability_mask2.........0x%X\n"
			"\t\t\t\tlink_speed_ext_active....0x%X\n"
			"\t\t\t\tlink_speed_ext_supported.0x%X\n"
			"\t\t\t\tlink_speed_ext_enabled...0x%X\n",
			port_num, cl_ntoh64(node_guid), cl_ntoh64(port_guid),
			cl_ntoh64(p_pi->m_key), cl_ntoh64(p_pi->subnet_prefix),
			cl_ntoh16(p_pi->base_lid),
			cl_ntoh16(p_pi->master_sm_base_lid),
			cl_ntoh32(p_pi->capability_mask),
			cl_ntoh16(p_pi->diag_code),
			cl_ntoh16(p_pi->m_key_lease_period),
			p_pi->local_port_num, p_pi->link_width_enabled,
			p_pi->link_width_supported, p_pi->link_width_active,
			ib_port_info_get_link_speed_sup(p_pi),
			ib_get_port_state_str(ib_port_info_get_port_state
					      (p_pi)), p_pi->state_info2,
			ib_port_info_get_mpb(p_pi), ib_port_info_get_lmc(p_pi),
			p_pi->link_speed, p_pi->mtu_smsl, p_pi->vl_cap,
			p_pi->vl_high_limit, p_pi->vl_arb_high_cap,
			p_pi->vl_arb_low_cap, p_pi->mtu_cap,
			p_pi->vl_stall_life, p_pi->vl_enforce,
			cl_ntoh16(p_pi->m_key_violations),
			cl_ntoh16(p_pi->p_key_violations),
			cl_ntoh16(p_pi->q_key_violations), p_pi->guid_cap,
			ib_port_info_get_client_rereg(p_pi),
			ib_port_info_get_mcast_pkey_trap_suppress(p_pi),
			ib_port_info_get_timeout(p_pi),
			ib_port_info_get_resp_time_value(p_pi),
			p_pi->error_threshold, cl_ntoh16(p_pi->max_credit_hint),
			cl_ntoh32(p_pi->link_rt_latency),
			cl_ntoh16(p_pi->capability_mask2),
			ib_port_info_get_link_speed_ext_active(p_pi),
			ib_port_info_get_link_speed_ext_sup(p_pi),
			p_pi->link_speed_ext_enabled);
	}
}