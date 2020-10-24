static int link_mgr_process_node(osm_sm_t * sm, IN osm_node_t * p_node,
				 IN const uint8_t link_state)
{
	osm_physp_t *p_physp, *p_physp_remote;
	uint32_t i, num_physp;
	int ret = 0;
	uint8_t current_state;

	OSM_LOG_ENTER(sm->p_log);

	OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
		"Node 0x%" PRIx64 " going to %s\n",
		cl_ntoh64(osm_node_get_node_guid(p_node)),
		ib_get_port_state_str(link_state));

	/*
	   Set the PortInfo for every Physical Port associated
	   with this Port.  Start iterating with port 1, since the linkstate
	   is not applicable to the management port on switches.
	 */
	num_physp = osm_node_get_num_physp(p_node);
	for (i = 0; i < num_physp; i++) {
		/*
		   Don't bother doing anything if this Physical Port is not valid.
		   or if the state of the port is already better then the
		   specified state.
		 */
		p_physp = osm_node_get_physp_ptr(p_node, (uint8_t) i);
		if (!p_physp)
			continue;

		current_state = osm_physp_get_port_state(p_physp);
		if (current_state == IB_LINK_DOWN)
			continue;

		/*
		    Set PortState to DOWN in case Remote Physical Port is
		    unreachable. We have to check this for all ports, except
		    port zero.
		 */
		p_physp_remote = osm_physp_get_remote(p_physp);
		if ((i != 0) && (!p_physp_remote ||
		    !osm_physp_is_valid(p_physp_remote))) {
			if (current_state != IB_LINK_INIT)
				link_mgr_set_physp_pi(sm, p_physp, IB_LINK_DOWN);
			continue;
		}

		/*
		   Normally we only send state update if state is lower
		   then required state. However, we need to send update if
		   no state change required.
		 */
		if (link_state != IB_LINK_NO_CHANGE &&
		    link_state <= current_state)
			OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
				"Physical port %u already %s. Skipping\n",
				p_physp->port_num,
				ib_get_port_state_str(current_state));
		else if (link_mgr_set_physp_pi(sm, p_physp, link_state))
			ret = -1;
	}

	OSM_LOG_EXIT(sm->p_log);
	return ret;
}