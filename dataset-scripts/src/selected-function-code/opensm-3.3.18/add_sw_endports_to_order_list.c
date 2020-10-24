static void add_sw_endports_to_order_list(osm_switch_t * sw,
					  osm_ucast_mgr_t * m,
					  cl_qmap_t * guid_tbl,
					  boolean_t add_guids)
{
	osm_port_t *port;
	uint64_t port_guid;
	osm_physp_t *p;
	int i;
	boolean_t found;

	for (i = 1; i < sw->num_ports; i++) {
		p = osm_node_get_physp_ptr(sw->p_node, i);
		if (p && p->p_remote_physp && !p->p_remote_physp->p_node->sw) {
			port_guid = p->p_remote_physp->port_guid;
			port = osm_get_port_by_guid(m->p_subn, port_guid);
			if (!port)
				continue;
			if (!cl_is_qmap_empty(guid_tbl)) {
				found = (cl_qmap_get(guid_tbl, port_guid)
					 != cl_qmap_end(guid_tbl));
				if ((add_guids && !found)
				    || (!add_guids && found))
					continue;
			}
			if (!cl_is_item_in_qlist(&m->port_order_list,
						 &port->list_item))
				cl_qlist_insert_tail(&m->port_order_list,
						     &port->list_item);
			else
				OSM_LOG(m->p_log, OSM_LOG_INFO,
					"WRN AD37: guid 0x%016" PRIx64
					" already in list\n", port_guid);
		}
	}
}