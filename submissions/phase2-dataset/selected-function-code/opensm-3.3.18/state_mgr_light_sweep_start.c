static ib_api_status_t state_mgr_light_sweep_start(IN osm_sm_t * sm)
{
	ib_api_status_t status = IB_SUCCESS;
	osm_bind_handle_t h_bind;
	cl_qmap_t *p_sw_tbl;
	cl_map_item_t *p_next;
	osm_node_t *p_node;
	osm_physp_t *p_physp;
	uint8_t port_num;

	OSM_LOG_ENTER(sm->p_log);

	p_sw_tbl = &sm->p_subn->sw_guid_tbl;

	/*
	 * First, get the bind handle.
	 */
	h_bind = osm_sm_mad_ctrl_get_bind_handle(&sm->mad_ctrl);
	if (h_bind == OSM_BIND_INVALID_HANDLE) {
		OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
			"No bound ports. Deferring sweep...\n");
		status = IB_INVALID_STATE;
		goto _exit;
	}

	OSM_LOG_MSG_BOX(sm->p_log, OSM_LOG_VERBOSE, "INITIATING LIGHT SWEEP");
	CL_PLOCK_ACQUIRE(sm->p_lock);
	cl_qmap_apply_func(p_sw_tbl, state_mgr_get_sw_info, sm);
	CL_PLOCK_RELEASE(sm->p_lock);

	CL_PLOCK_ACQUIRE(sm->p_lock);
	cl_qmap_apply_func(&sm->p_subn->node_guid_tbl, state_mgr_get_node_desc,
			   sm);
	CL_PLOCK_RELEASE(sm->p_lock);

	/* now scan the list of physical ports that were not down but have no remote port */
	CL_PLOCK_ACQUIRE(sm->p_lock);
	p_next = cl_qmap_head(&sm->p_subn->node_guid_tbl);
	while (p_next != cl_qmap_end(&sm->p_subn->node_guid_tbl)) {
		p_node = (osm_node_t *) p_next;
		p_next = cl_qmap_next(p_next);

		for (port_num = 1; port_num < osm_node_get_num_physp(p_node);
		     port_num++) {
			p_physp = osm_node_get_physp_ptr(p_node, port_num);
			if (p_physp && (osm_physp_get_port_state(p_physp) !=
					IB_LINK_DOWN)
			    && !osm_physp_get_remote(p_physp)) {
				OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3315: "
					"Unknown remote side for node 0x%016"
					PRIx64
					" (%s) port %u. Adding to light sweep sampling list\n",
					cl_ntoh64(osm_node_get_node_guid
						  (p_node)),
					p_node->print_desc, port_num);

				osm_dump_dr_path_v2(sm->p_log,
						    osm_physp_get_dr_path_ptr
						    (p_physp), FILE_ID, OSM_LOG_ERROR);

				state_mgr_get_remote_port_info(sm, p_physp);
			}
		}
	}

	cl_qmap_apply_func(&sm->p_subn->sm_guid_tbl, query_sm_info, sm);

	CL_PLOCK_RELEASE(sm->p_lock);

_exit:
	OSM_LOG_EXIT(sm->p_log);
	return status;
}