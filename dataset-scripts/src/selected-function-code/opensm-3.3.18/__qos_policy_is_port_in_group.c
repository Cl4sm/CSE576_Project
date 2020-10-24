static boolean_t
__qos_policy_is_port_in_group(osm_subn_t * p_subn,
			      const osm_physp_t * p_physp,
			      osm_qos_port_group_t * p_port_group)
{
	osm_node_t *p_node = osm_physp_get_node_ptr(p_physp);
	ib_net64_t port_guid = osm_physp_get_port_guid(p_physp);
	uint64_t port_guid_ho = cl_ntoh64(port_guid);

	/* check whether this port's type matches any of group's types */

	if ( p_port_group->node_types &
	     (((uint8_t)1)<<osm_node_get_type(p_node)) )
		return TRUE;

	/* check whether this port's guid is in group's port map */

	if (cl_qmap_get(&p_port_group->port_map, port_guid_ho) !=
	    cl_qmap_end(&p_port_group->port_map))
		return TRUE;

	return FALSE;
}				/* __qos_policy_is_port_in_group() */