uint8_t osm_switch_get_port_least_hops(IN const osm_switch_t * p_sw,
				       IN const osm_port_t * p_port)
{
	uint16_t lid;

	if (p_port->p_node->sw) {
		if (p_port->p_node->sw == p_sw)
			return 0;
		lid = osm_node_get_base_lid(p_port->p_node, 0);
		return osm_switch_get_least_hops(p_sw, cl_ntoh16(lid));
	} else {
		osm_physp_t *p = p_port->p_physp;
		uint8_t hops;

		if (!p || !p->p_remote_physp || !p->p_remote_physp->p_node->sw)
			return OSM_NO_PATH;
		if (p->p_remote_physp->p_node->sw == p_sw)
			return 1;
		lid = osm_node_get_base_lid(p->p_remote_physp->p_node, 0);
		hops = osm_switch_get_least_hops(p_sw, cl_ntoh16(lid));
		return hops != OSM_NO_PATH ? hops + 1 : OSM_NO_PATH;
	}
}