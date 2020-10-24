int osm_mcast_make_port_list_and_map(cl_qlist_t * list, cl_qmap_t * map,
				     osm_mgrp_box_t * mbox)
{
	cl_map_item_t *map_item;
	cl_list_item_t *list_item;
	osm_mgrp_t *mgrp;
	osm_mcm_port_t *mcm_port;
	osm_mcast_work_obj_t *wobj;

	cl_qmap_init(map);
	cl_qlist_init(list);

	for (list_item = cl_qlist_head(&mbox->mgrp_list);
	     list_item != cl_qlist_end(&mbox->mgrp_list);
	     list_item = cl_qlist_next(list_item)) {
		mgrp = cl_item_obj(list_item, mgrp, list_item);
		for (map_item = cl_qmap_head(&mgrp->mcm_port_tbl);
		     map_item != cl_qmap_end(&mgrp->mcm_port_tbl);
		     map_item = cl_qmap_next(map_item)) {
			/* Acquire the port object for this port guid, then
			   create the new worker object to build the list. */
			mcm_port = cl_item_obj(map_item, mcm_port, map_item);
			if (cl_qmap_get(map, mcm_port->port->guid) !=
			    cl_qmap_end(map))
				continue;
			wobj = mcast_work_obj_new(mcm_port->port);
			if (!wobj)
				return -1;
			cl_qlist_insert_tail(list, &wobj->list_item);
			cl_qmap_insert(map, mcm_port->port->guid,
				       &wobj->map_item);
		}
	}
	return 0;
}