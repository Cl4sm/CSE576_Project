static boolean_t state_mgr_is_sm_port_down(IN osm_sm_t * sm)
{
	ib_net64_t port_guid;
	osm_port_t *p_port;
	osm_physp_t *p_physp;
	uint8_t state;

	OSM_LOG_ENTER(sm->p_log);

	port_guid = sm->p_subn->sm_port_guid;

	/*
	 * If we don't know our own port guid yet, assume the port is down.
	 */
	if (port_guid == 0) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3308: "
			"SM port GUID unknown\n");
		state = IB_LINK_DOWN;
		goto Exit;
	}

	CL_ASSERT(port_guid);

	CL_PLOCK_ACQUIRE(sm->p_lock);
	p_port = osm_get_port_by_guid(sm->p_subn, port_guid);
	if (!p_port) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3309: "
			"SM port with GUID:%016" PRIx64 " is unknown\n",
			cl_ntoh64(port_guid));
		state = IB_LINK_DOWN;
		CL_PLOCK_RELEASE(sm->p_lock);
		goto Exit;
	}

	p_physp = p_port->p_physp;

	CL_ASSERT(p_physp);

	if (p_port->p_node->sw &&
	    !ib_switch_info_is_enhanced_port0(&p_port->p_node->sw->switch_info))
		state = IB_LINK_ACTIVE;	/* base SP0 */
	else
		state = osm_physp_get_port_state(p_physp);
	CL_PLOCK_RELEASE(sm->p_lock);

Exit:
	OSM_LOG_EXIT(sm->p_log);
	return (state == IB_LINK_DOWN);
}