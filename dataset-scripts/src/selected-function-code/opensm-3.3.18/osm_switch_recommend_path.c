uint8_t osm_switch_recommend_path(IN const osm_switch_t * p_sw,
				  IN osm_port_t * p_port, IN uint16_t lid_ho,
				  IN unsigned start_from,
				  IN boolean_t ignore_existing,
				  IN boolean_t routing_for_lmc,
				  IN boolean_t dor,
				  IN boolean_t port_shifting,
				  IN uint32_t scatter_ports)
{
	/*
	   We support an enhanced LMC aware routing mode:
	   In the case of LMC > 0, we can track the remote side
	   system and node for all of the lids of the target
	   and try and avoid routing again through the same
	   system / node.

	   Assume if routing_for_lmc is true that this procedure was
	   provided the tracking array and counter via p_port->priv,
	   and we can conduct this algorithm.
	 */
	uint16_t base_lid;
	uint8_t hops;
	uint8_t least_hops;
	uint8_t port_num;
	uint8_t num_ports;
	uint32_t least_paths = 0xFFFFFFFF;
	unsigned i;
	/*
	   The follwing will track the least paths if the
	   route should go through a new system/node
	 */
	uint32_t least_paths_other_sys = 0xFFFFFFFF;
	uint32_t least_paths_other_nodes = 0xFFFFFFFF;
	uint32_t least_forwarded_to = 0xFFFFFFFF;
	uint32_t check_count;
	uint8_t best_port = 0;
	/*
	   These vars track the best port if it connects to
	   not used system/node.
	 */
	uint8_t best_port_other_sys = 0;
	uint8_t best_port_other_node = 0;
	boolean_t port_found = FALSE;
	osm_physp_t *p_physp;
	osm_physp_t *p_rem_physp;
	osm_node_t *p_rem_node;
	osm_node_t *p_rem_node_first = NULL;
	struct osm_remote_node *p_remote_guid = NULL;
	struct osm_remote_node null_remote_node = {NULL, 0, 0};
	struct switch_port_path port_paths[IB_NODE_NUM_PORTS_MAX];
	unsigned int port_paths_total_paths = 0;
	unsigned int port_paths_count = 0;
	uint8_t scatter_possible_ports[IB_NODE_NUM_PORTS_MAX];
	unsigned int scatter_possible_ports_count = 0;
	int found_sys_guid = 0;
	int found_node_guid = 0;

	CL_ASSERT(lid_ho > 0);

	if (p_port->p_node->sw) {
		if (p_port->p_node->sw == p_sw)
			return 0;
		base_lid = osm_port_get_base_lid(p_port);
	} else {
		p_physp = p_port->p_physp;
		if (!p_physp || !p_physp->p_remote_physp ||
		    !p_physp->p_remote_physp->p_node->sw)
			return OSM_NO_PATH;

		if (p_physp->p_remote_physp->p_node->sw == p_sw)
			return p_physp->p_remote_physp->port_num;
		base_lid =
		    osm_node_get_base_lid(p_physp->p_remote_physp->p_node, 0);
	}
	base_lid = cl_ntoh16(base_lid);

	num_ports = p_sw->num_ports;

	least_hops = osm_switch_get_least_hops(p_sw, base_lid);
	if (least_hops == OSM_NO_PATH)
		return OSM_NO_PATH;

	/*
	   First, inquire with the forwarding table for an existing
	   route.  If one is found, honor it unless:
	   1. the ignore existing flag is set.
	   2. the physical port is not a valid one or not healthy
	   3. the physical port has a remote port (the link is up)
	   4. the port has min-hops to the target (avoid loops)
	 */
	if (!ignore_existing) {
		port_num = osm_switch_get_port_by_lid(p_sw, lid_ho);

		if (port_num != OSM_NO_PATH) {
			CL_ASSERT(port_num < num_ports);

			p_physp =
			    osm_node_get_physp_ptr(p_sw->p_node, port_num);
			/*
			   Don't be too trusting of the current forwarding table!
			   Verify that the port number is legal and that the
			   LID is reachable through this port.
			 */
			if (p_physp && osm_physp_is_healthy(p_physp) &&
			    osm_physp_get_remote(p_physp)) {
				hops =
				    osm_switch_get_hop_count(p_sw, base_lid,
							     port_num);
				/*
				   If we aren't using pre-defined user routes
				   function, then we need to make sure that the
				   current path is the minimum one. In case of
				   having such a user function - this check will
				   not be done, and the old routing will be used.
				   Note: This means that it is the user's job to
				   clean all data in the forwarding tables that
				   he wants to be overridden by the minimum
				   hop function.
				 */
				if (hops == least_hops)
					return port_num;
			}
		}
	}

	/*
	   This algorithm selects a port based on a static load balanced
	   selection across equal hop-count ports.
	   There is lots of room for improved sophistication here,
	   possibly guided by user configuration info.
	 */

	/*
	   OpenSM routing is "local" - not considering a full lid to lid
	   path. As such we can not guarantee a path will not loop if we
	   do not always follow least hops.
	   So we must abort if not least hops.
	 */

	/* port number starts with one and num_ports is 1 + num phys ports */
	for (i = start_from; i < start_from + num_ports; i++) {
		port_num = osm_switch_get_dimn_port(p_sw, i % num_ports);
		if (!port_num ||
		    osm_switch_get_hop_count(p_sw, base_lid, port_num) !=
		    least_hops)
			continue;

		/* let us make sure it is not down or unhealthy */
		p_physp = osm_node_get_physp_ptr(p_sw->p_node, port_num);
		if (!p_physp || !osm_physp_is_healthy(p_physp) ||
		    /*
		       we require all - non sma ports to be linked
		       to be routed through
		     */
		    !osm_physp_get_remote(p_physp))
			continue;

		/*
		   We located a least-hop port, possibly one of many.
		   For this port, check the running total count of
		   the number of paths through this port.  Select
		   the port routing the least number of paths.
		 */
		check_count =
		    osm_port_prof_path_count_get(&p_sw->p_prof[port_num]);


		if (dor) {
			/* Get the Remote Node */
			p_rem_physp = osm_physp_get_remote(p_physp);
			p_rem_node = osm_physp_get_node_ptr(p_rem_physp);
			/* use the first dimension, but spread traffic
			 * out among the group of ports representing
			 * that dimension */
			if (!p_rem_node_first)
				p_rem_node_first = p_rem_node;
			else if (p_rem_node != p_rem_node_first)
				continue;
			if (routing_for_lmc) {
				struct osm_remote_guids_count *r = p_port->priv;
				uint8_t rem_port = osm_physp_get_port_num(p_rem_physp);
				unsigned int j;

				for (j = 0; j < r->count; j++) {
					p_remote_guid = &r->guids[j];
					if ((p_remote_guid->node == p_rem_node)
					    && (p_remote_guid->port == rem_port))
						break;
				}
				if (j == r->count)
					p_remote_guid = &null_remote_node;
			}
		/*
		   Advanced LMC routing requires tracking of the
		   best port by the node connected to the other side of
		   it.
		 */
		} else if (routing_for_lmc) {
			/* Is the sys guid already used ? */
			p_remote_guid = switch_find_sys_guid_count(p_sw,
								   p_port->priv,
								   port_num);

			/* If not update the least hops for this case */
			if (!p_remote_guid) {
				if (check_count < least_paths_other_sys) {
					least_paths_other_sys = check_count;
					best_port_other_sys = port_num;
					least_forwarded_to = 0;
				}
				found_sys_guid = 0;
			} else {	/* same sys found - try node */


				/* Else is the node guid already used ? */
				p_remote_guid = switch_find_node_guid_count(p_sw,
									    p_port->priv,
									    port_num);

				/* If not update the least hops for this case */
				if (!p_remote_guid
				    && check_count < least_paths_other_nodes) {
					least_paths_other_nodes = check_count;
					best_port_other_node = port_num;
					least_forwarded_to = 0;
				}
				/* else prior sys and node guid already used */

				if (!p_remote_guid)
					found_node_guid = 0;
				else
					found_node_guid = 1;
				found_sys_guid = 1;
			}	/* same sys found */
		}

		port_paths[port_paths_count].port_num = port_num;
		port_paths[port_paths_count].path_count = check_count;
		if (routing_for_lmc) {
			port_paths[port_paths_count].found_sys_guid = found_sys_guid;
			port_paths[port_paths_count].found_node_guid = found_node_guid;
		}
		if (routing_for_lmc && p_remote_guid)
			port_paths[port_paths_count].forwarded_to = p_remote_guid->forwarded_to;
		else
			port_paths[port_paths_count].forwarded_to = 0;
		port_paths_total_paths += check_count;
		port_paths_count++;

		/* routing for LMC mode */
		/*
		   the count is min but also lower then the max subscribed
		 */
		if (check_count < least_paths) {
			port_found = TRUE;
			best_port = port_num;
			least_paths = check_count;
			scatter_possible_ports_count = 0;
			scatter_possible_ports[scatter_possible_ports_count++] = port_num;
			if (routing_for_lmc
			    && p_remote_guid
			    && p_remote_guid->forwarded_to < least_forwarded_to)
				least_forwarded_to = p_remote_guid->forwarded_to;
		} else if (scatter_ports
			   && check_count == least_paths) {
			scatter_possible_ports[scatter_possible_ports_count++] = port_num;
		} else if (routing_for_lmc
			   && p_remote_guid
			   && check_count == least_paths
			   && p_remote_guid->forwarded_to < least_forwarded_to) {
			least_forwarded_to = p_remote_guid->forwarded_to;
			best_port = port_num;
		}
	}

	if (port_found == FALSE)
		return OSM_NO_PATH;

	if (port_shifting && port_paths_count) {
		/* In the port_paths[] array, we now have all the ports that we
		 * can route out of.  Using some shifting math below, possibly
		 * select a different one so that lids won't align in LFTs
		 *
		 * If lmc > 0, we need to loop through these ports to find the
		 * least_forwarded_to port, best_port_other_sys, and
		 * best_port_other_node just like before but through the different
		 * ordering.
		 */

		least_paths = 0xFFFFFFFF;
		least_paths_other_sys = 0xFFFFFFFF;
		least_paths_other_nodes = 0xFFFFFFFF;
	        least_forwarded_to = 0xFFFFFFFF;
		best_port = 0;
		best_port_other_sys = 0;
		best_port_other_node = 0;

		for (i = 0; i < port_paths_count; i++) {
			unsigned int idx;

			idx = (port_paths_total_paths/port_paths_count + i) % port_paths_count;

			if (routing_for_lmc) {
				if (!port_paths[idx].found_sys_guid
				    && port_paths[idx].path_count < least_paths_other_sys) {
					least_paths_other_sys = port_paths[idx].path_count;
					best_port_other_sys = port_paths[idx].port_num;
					least_forwarded_to = 0;
				}
				else if (!port_paths[idx].found_node_guid
					 && port_paths[idx].path_count < least_paths_other_nodes) {
					least_paths_other_nodes = port_paths[idx].path_count;
					best_port_other_node = port_paths[idx].port_num;
					least_forwarded_to = 0;
				}
			}

			if (port_paths[idx].path_count < least_paths) {
				best_port = port_paths[idx].port_num;
				least_paths = port_paths[idx].path_count;
				if (routing_for_lmc
				    && (port_paths[idx].found_sys_guid
					|| port_paths[idx].found_node_guid)
				    && port_paths[idx].forwarded_to < least_forwarded_to)
					least_forwarded_to = port_paths[idx].forwarded_to;
			}
			else if (routing_for_lmc
				 && (port_paths[idx].found_sys_guid
				     || port_paths[idx].found_node_guid)
				 && port_paths[idx].path_count == least_paths
				 && port_paths[idx].forwarded_to < least_forwarded_to) {
				least_forwarded_to = port_paths[idx].forwarded_to;
				best_port = port_paths[idx].port_num;
			}

		}
	}

	/*
	   if we are in enhanced routing mode and the best port is not
	   the local port 0
	 */
	if (routing_for_lmc && best_port && !scatter_ports) {
		/* Select the least hop port of the non used sys first */
		if (best_port_other_sys)
			best_port = best_port_other_sys;
		else if (best_port_other_node)
			best_port = best_port_other_node;
	} else if (scatter_ports) {
		/*
		 * There is some danger that this random could "rebalance" the routes
		 * every time, to combat this there is a global srandom that
		 * occurs at the start of every sweep.
		 */
		unsigned int idx = random() % scatter_possible_ports_count;
		best_port = scatter_possible_ports[idx];
	}
	return best_port;
}