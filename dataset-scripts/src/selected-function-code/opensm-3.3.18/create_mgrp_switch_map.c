static void create_mgrp_switch_map(cl_qmap_t * m, cl_qlist_t * port_list)
{
	osm_mcast_work_obj_t *wobj;
	osm_port_t *port;
	osm_switch_t *sw;
	ib_net64_t guid;
	cl_list_item_t *i;

	cl_qmap_init(m);
	for (i = cl_qlist_head(port_list); i != cl_qlist_end(port_list);
	     i = cl_qlist_next(i)) {
		wobj = cl_item_obj(i, wobj, list_item);
		port = wobj->p_port;
		if (port->p_node->sw) {
			sw = port->p_node->sw;
			sw->is_mc_member = 1;
		} else {
			sw = port->p_physp->p_remote_physp->p_node->sw;
			sw->num_of_mcm++;
		}
		guid = osm_node_get_node_guid(sw->p_node);
		if (cl_qmap_get(m, guid) == cl_qmap_end(m))
			cl_qmap_insert(m, guid, &sw->mgrp_item);
	}
}