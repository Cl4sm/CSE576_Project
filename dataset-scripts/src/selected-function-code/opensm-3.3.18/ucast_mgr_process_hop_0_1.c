static void ucast_mgr_process_hop_0_1(IN cl_map_item_t * p_map_item,
				      IN void *context)
{
	osm_switch_t * p_sw = (osm_switch_t *) p_map_item;
	osm_node_t *p_remote_node;
	uint16_t lid, remote_lid;
	uint8_t i;

	lid = cl_ntoh16(osm_node_get_base_lid(p_sw->p_node, 0));
	osm_switch_set_hops(p_sw, lid, 0, 0);

	for (i = 1; i < p_sw->num_ports; i++) {
		osm_physp_t *p = osm_node_get_physp_ptr(p_sw->p_node, i);
		p_remote_node = (p && p->p_remote_physp) ?
		    p->p_remote_physp->p_node : NULL;

		if (p_remote_node && p_remote_node->sw &&
		    p_remote_node != p_sw->p_node) {
			remote_lid = osm_node_get_base_lid(p_remote_node, 0);
			remote_lid = cl_ntoh16(remote_lid);
			osm_switch_set_hops(p_sw, remote_lid, i, p->hop_wf);
		}
	}
}