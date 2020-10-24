static int dnup_subn_rank(IN dnup_t * p_dnup)
{
	osm_switch_t *p_sw;
	osm_physp_t *p_physp, *p_remote_physp;
	cl_qlist_t list;
	cl_map_item_t *item;
	struct dnup_node *u, *remote_u;
	uint8_t num_ports, port_num;
	osm_log_t *p_log = &p_dnup->p_osm->log;
	unsigned max_rank = 0;

	OSM_LOG_ENTER(p_log);
	cl_qlist_init(&list);

	/* add all node level switches to the list */
	for (item = cl_qmap_head(&p_dnup->p_osm->subn.sw_guid_tbl);
	     item != cl_qmap_end(&p_dnup->p_osm->subn.sw_guid_tbl);
	     item = cl_qmap_next(item)) {
		p_sw = (osm_switch_t *)item;
		u = p_sw->priv;
		if (u->rank == 0)
			cl_qlist_insert_tail(&list, &u->list);
	}

	/* BFS the list till it's empty */
	while (!cl_is_qlist_empty(&list)) {
		u = (struct dnup_node *)cl_qlist_remove_head(&list);
		/* Go over all remote nodes and rank them (if not already visited) */
		p_sw = u->sw;
		num_ports = p_sw->num_ports;
		OSM_LOG(p_log, OSM_LOG_DEBUG,
			"Handling switch GUID 0x%" PRIx64 "\n",
			cl_ntoh64(osm_node_get_node_guid(p_sw->p_node)));
		for (port_num = 1; port_num < num_ports; port_num++) {
			ib_net64_t port_guid;

			/* Current port fetched in order to get remote side */
			p_physp =
			    osm_node_get_physp_ptr(p_sw->p_node, port_num);

			if (!p_physp)
				continue;

			p_remote_physp = p_physp->p_remote_physp;

			/*
			   make sure that all the following occur on p_remote_physp:
			   1. The port isn't NULL
			   2. It is a switch
			 */
			if (p_remote_physp && p_remote_physp->p_node->sw) {
				remote_u = p_remote_physp->p_node->sw->priv;
				port_guid = p_remote_physp->port_guid;

				if (remote_u->rank > u->rank + 1) {
					remote_u->rank = u->rank + 1;
					max_rank = remote_u->rank;
					cl_qlist_insert_tail(&list,
							     &remote_u->list);
					OSM_LOG(p_log, OSM_LOG_DEBUG,
						"Rank of port GUID 0x%" PRIx64
						" = %u\n", cl_ntoh64(port_guid),
						remote_u->rank);
				}
			}
		}
	}

	/* Print Summary of ranking */
	OSM_LOG(p_log, OSM_LOG_VERBOSE,
		"Subnet ranking completed. Max Node Rank = %d\n", max_rank);
	OSM_LOG_EXIT(p_log);
	return 0;
}