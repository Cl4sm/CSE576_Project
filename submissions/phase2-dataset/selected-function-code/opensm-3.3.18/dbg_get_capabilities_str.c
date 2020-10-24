static void dbg_get_capabilities_str(IN char *p_buf, IN uint32_t buf_size,
				     IN const char *p_prefix_str,
				     IN const ib_port_info_t * p_pi)
{
	uint32_t total_len = 0;
	char *p_local = p_buf;

	strcpy(p_local, "Capability Mask:\n");
	p_local += strlen(p_local);

	if (p_pi->capability_mask & IB_PORT_CAP_RESV0) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_RESV0\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_IS_SM) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_IS_SM\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_HAS_NOTICE) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_HAS_NOTICE\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_HAS_TRAP) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_HAS_TRAP\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_HAS_IPD) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_HAS_IPD\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_HAS_AUTO_MIG) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_HAS_AUTO_MIG\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_HAS_SL_MAP) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_HAS_SL_MAP\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_HAS_NV_MKEY) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_HAS_NV_MKEY\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_HAS_NV_PKEY) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_HAS_NV_PKEY\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_HAS_LED_INFO) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_HAS_LED_INFO\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_SM_DISAB) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_SM_DISAB\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_HAS_SYS_IMG_GUID) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_HAS_SYS_IMG_GUID\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_HAS_PKEY_SW_EXT_PORT_TRAP) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_PKEY_SW_EXT_PORT_TRAP\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_RESV13) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_RESV13\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_HAS_EXT_SPEEDS) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_HAS_EXT_SPEEDS\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_HAS_CAP_MASK2) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_HAS_CAP_MASK2\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_HAS_COM_MGT) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_HAS_COM_MGT\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_HAS_SNMP) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_HAS_SNMP\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_REINIT) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_REINIT\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_HAS_DEV_MGT) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_HAS_DEV_MGT\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_HAS_VEND_CLS) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_HAS_VEND_CLS\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_HAS_DR_NTC) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_HAS_DR_NTC\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_HAS_CAP_NTC) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_HAS_CAP_NTC\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_HAS_BM) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_HAS_BM\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_HAS_LINK_RT_LATENCY) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_HAS_LINK_RT_LATENCY\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_HAS_CLIENT_REREG) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_HAS_CLIENT_REREG\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_HAS_OTHER_LOCAL_CHANGES_NTC) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_HAS_OTHER_LOCAL_CHANGES_NTC\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_HAS_LINK_SPEED_WIDTH_PAIRS_TBL) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_HAS_LINK_SPEED_WIDTH_PAIRS_TBL\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_HAS_VEND_MADS) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_HAS_VEND_MADS\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_HAS_MCAST_PKEY_TRAP_SUPPRESS) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_HAS_MCAST_PKEY_TRAP_SUPPRESS\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_HAS_MCAST_FDB_TOP) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_HAS_MCAST_FDB_TOP\n",
				&total_len) != IB_SUCCESS)
			return;
	}
	if (p_pi->capability_mask & IB_PORT_CAP_HAS_HIER_INFO) {
		if (dbg_do_line(&p_local, buf_size, p_prefix_str,
				"IB_PORT_CAP_HAS_HIER_INFO\n",
				&total_len) != IB_SUCCESS)
			return;
	}
}