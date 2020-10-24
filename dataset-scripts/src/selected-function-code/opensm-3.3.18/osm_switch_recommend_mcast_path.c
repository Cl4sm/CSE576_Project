uint8_t osm_switch_recommend_mcast_path(IN osm_switch_t * p_sw,
					IN osm_port_t * p_port,
					IN uint16_t mlid_ho,
					IN boolean_t ignore_existing)
{
	uint16_t base_lid;
	uint8_t hops;
	uint8_t port_num;
	uint8_t num_ports;
	uint8_t least_hops;

	CL_ASSERT(mlid_ho >= IB_LID_MCAST_START_HO);

	if (p_port->p_node->sw) {
		if (p_port->p_node->sw == p_sw)
			return 0;
		base_lid = osm_port_get_base_lid(p_port);
	} else {
		osm_physp_t *p_physp = p_port->p_physp;
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

	/*
	   If the user wants us to ignore existing multicast routes,
	   then simply return the shortest hop count path to the
	   target port.

	   Otherwise, return the first port that has a path to the target,
	   picking from the ports that are already in the multicast group.
	 */
	if (!ignore_existing) {
		for (port_num = 1; port_num < num_ports; port_num++) {
			if (!osm_mcast_tbl_is_port
			    (&p_sw->mcast_tbl, mlid_ho, port_num))
				continue;
			/*
			   Don't be too trusting of the current forwarding table!
			   Verify that the LID is reachable through this port.
			 */
			hops =
			    osm_switch_get_hop_count(p_sw, base_lid, port_num);
			if (hops != OSM_NO_PATH)
				return port_num;
		}
	}

	/*
	   Either no existing mcast paths reach this port or we are
	   ignoring existing paths.

	   Determine the best multicast path to the target.  Note that this
	   algorithm is slightly different from the one used for unicast route
	   recommendation.  In this case (multicast), we must NOT
	   perform any sort of load balancing.  We MUST take the FIRST
	   port found that has <= the lowest hop count path.  This prevents
	   more than one multicast path to the same remote switch which
	   prevents a multicast loop.  Multicast loops are bad since the same
	   multicast packet will go around and around, inevitably creating
	   a black hole that will destroy the Earth in a firey conflagration.
	 */
	least_hops = osm_switch_get_least_hops(p_sw, base_lid);
	if (least_hops == OSM_NO_PATH)
		return OSM_NO_PATH;
	for (port_num = 1; port_num < num_ports; port_num++)
		if (osm_switch_get_hop_count(p_sw, base_lid, port_num) ==
		    least_hops)
			break;

	CL_ASSERT(port_num < num_ports);
	return port_num;
}