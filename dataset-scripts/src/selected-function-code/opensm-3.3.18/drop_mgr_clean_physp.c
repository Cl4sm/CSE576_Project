static void drop_mgr_clean_physp(osm_sm_t * sm, IN osm_physp_t * p_physp)
{
	osm_physp_t *p_remote_physp;
	osm_port_t *p_remote_port;

	p_remote_physp = osm_physp_get_remote(p_physp);
	if (p_remote_physp) {
		p_remote_port = osm_get_port_by_guid(sm->p_subn,
						     p_remote_physp->port_guid);

		if (p_remote_port) {
			/* Let's check if this is a case of link that is lost
			   (both ports weren't recognized), or a "hiccup" in the
			   subnet - in which case the remote port was
			   recognized, and its state is ACTIVE.
			   If this is just a "hiccup" - force a heavy sweep in
			   the next sweep. We don't want to lose that part of
			   the subnet. */
			if (p_remote_port->discovery_count &&
			    osm_physp_get_port_state(p_remote_physp) ==
			    IB_LINK_ACTIVE) {
				OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
					"Forcing new heavy sweep. Remote "
					"port 0x%016" PRIx64 " port num: %u "
					"was recognized in ACTIVE state\n",
					cl_ntoh64(p_remote_physp->port_guid),
					p_remote_physp->port_num);
				sm->p_subn->force_heavy_sweep = TRUE;
			}

			/* If the remote node is ca or router - need to remove
			   the remote port, since it is no longer reachable.
			   This can be done if we reset the discovery count
			   of the remote port. */
			if (!p_remote_physp->p_node->sw) {
				p_remote_port->discovery_count = 0;
				OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
					"Resetting discovery count of node: "
					"0x%016" PRIx64 " port num:%u\n",
					cl_ntoh64(osm_node_get_node_guid
						  (p_remote_physp->p_node)),
					p_remote_physp->port_num);
			}
		}

		OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
			"Unlinking local node 0x%016" PRIx64 ", port %u"
			"\n\t\t\t\tand remote node 0x%016" PRIx64
			", port %u\n",
			cl_ntoh64(osm_node_get_node_guid(p_physp->p_node)),
			p_physp->port_num,
			cl_ntoh64(osm_node_get_node_guid
				  (p_remote_physp->p_node)),
			p_remote_physp->port_num);

		if (sm->ucast_mgr.cache_valid)
			osm_ucast_cache_add_link(&sm->ucast_mgr, p_physp,
						 p_remote_physp);

		osm_physp_unlink(p_physp, p_remote_physp);

	}

	OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
		"Clearing node 0x%016" PRIx64 " physical port number %u\n",
		cl_ntoh64(osm_node_get_node_guid(p_physp->p_node)),
		p_physp->port_num);

	osm_physp_destroy(p_physp);
}