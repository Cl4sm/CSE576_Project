uint8_t osm_physp_calc_link_op_vls(IN osm_log_t * p_log,
				   IN const osm_subn_t * p_subn,
				   IN const osm_physp_t * p_physp,
				   IN uint8_t current_op_vls)
{
	const osm_physp_t *p_remote_physp;
	uint8_t op_vls;
	uint8_t remote_op_vls;

	OSM_LOG_ENTER(p_log);

	p_remote_physp = osm_physp_get_remote(p_physp);
	if (p_remote_physp) {
		/* use the available VLCap */
		op_vls = ib_port_info_get_vl_cap(&p_physp->port_info);

		remote_op_vls =
		    ib_port_info_get_vl_cap(&p_remote_physp->port_info);

		OSM_LOG(p_log, OSM_LOG_DEBUG,
			"Remote port 0x%016" PRIx64 " port = 0x%X : "
			"VL_CAP = %u. This port VL_CAP = %u\n",
			cl_ntoh64(osm_physp_get_port_guid(p_remote_physp)),
			osm_physp_get_port_num(p_remote_physp),
			remote_op_vls, op_vls);

		if (op_vls != remote_op_vls) {
			if (op_vls > remote_op_vls)
				op_vls = remote_op_vls;
			if (op_vls != current_op_vls)
				OSM_LOG(p_log, OSM_LOG_VERBOSE,
					"OP_VLS mismatch between ports."
					"\n\t\t\t\tPort 0x%016" PRIx64 ", port 0x%X"
					" and port 0x%016" PRIx64 ", port 0x%X."
					"\n\t\t\t\tUsing lower OP_VLS of %u\n",
					cl_ntoh64(osm_physp_get_port_guid(p_physp)),
					osm_physp_get_port_num(p_physp),
					cl_ntoh64(osm_physp_get_port_guid
						  (p_remote_physp)),
					osm_physp_get_port_num(p_remote_physp), op_vls);
		}
	} else
		op_vls = ib_port_info_get_op_vls(&p_physp->port_info);

	if (op_vls == 0) {
		/* for non compliant implementations */
		OSM_LOG(p_log, OSM_LOG_VERBOSE,
			"Invalid OP_VLS = 0. Forcing correction to 1 (VL0)\n");
		op_vls = 1;
	}

	/* support user limitation of max_op_vls */
	if (op_vls > p_subn->opt.max_op_vls)
		op_vls = p_subn->opt.max_op_vls;

	OSM_LOG_EXIT(p_log);
	return op_vls;
}