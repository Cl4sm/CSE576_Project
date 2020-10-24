static int dfsssp_do_dijkstra_routing(void *context)
{
	dfsssp_context_t *dfsssp_ctx = (dfsssp_context_t *) context;
	osm_ucast_mgr_t *p_mgr = (osm_ucast_mgr_t *) dfsssp_ctx->p_mgr;
	vertex_t *adj_list = (vertex_t *) dfsssp_ctx->adj_list;
	uint32_t adj_list_size = dfsssp_ctx->adj_list_size;

	vertex_t **sw_list = NULL;
	uint32_t sw_list_size = 0;
	uint64_t guid = 0;
	cl_qlist_t *qlist = NULL;
	cl_list_item_t *qlist_item = NULL;

	cl_qmap_t *sw_tbl = &p_mgr->p_subn->sw_guid_tbl;
	cl_qmap_t cn_tbl, io_tbl, *p_mixed_tbl = NULL;
	cl_map_item_t *item = NULL;
	osm_switch_t *sw = NULL;
	osm_port_t *port = NULL;
	uint32_t i = 0, err = 0;
	uint16_t lid = 0, min_lid_ho = 0, max_lid_ho = 0;
	uint8_t lmc = 0;
	boolean_t cn_nodes_provided = FALSE, io_nodes_provided = FALSE;

	OSM_LOG_ENTER(p_mgr->p_log);
	OSM_LOG(p_mgr->p_log, OSM_LOG_VERBOSE,
		"Calculating shortest path from all Hca/switches to all\n");

	cl_qmap_init(&cn_tbl);
	cl_qmap_init(&io_tbl);
	p_mixed_tbl = &cn_tbl;

	cl_qlist_init(&p_mgr->port_order_list);

	/* reset the new_lft for each switch */
	for (item = cl_qmap_head(sw_tbl); item != cl_qmap_end(sw_tbl);
	     item = cl_qmap_next(item)) {
		sw = (osm_switch_t *) item;
		/* initialize LIDs in buffer to invalid port number */
		memset(sw->new_lft, OSM_NO_PATH, sw->max_lid_ho + 1);
		/* initialize LFT and hop count for bsp0/esp0 of the switch */
		min_lid_ho = cl_ntoh16(osm_node_get_base_lid(sw->p_node, 0));
		lmc = osm_node_get_lmc(sw->p_node, 0);
		for (i = min_lid_ho; i < min_lid_ho + (1 << lmc); i++) {
			/* for each switch the port to the 'self'lid is the management port 0 */
			sw->new_lft[i] = 0;
			/* the hop count to the 'self'lid is 0 for each switch */
			osm_switch_set_hops(sw, i, 0, 0);
		}
	}

	/* we need an intermediate array of pointers to switches in adj_list;
	   this array will be sorted in respect to num_hca (descending)
	 */
	sw_list_size = adj_list_size - 1;
	sw_list = (vertex_t **)malloc(sw_list_size * sizeof(vertex_t *));
	if (!sw_list) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR,
			"ERR AD29: cannot allocate memory for sw_list in dfsssp_do_dijkstra_routing\n");
		goto ERROR;
	}
	memset(sw_list, 0, sw_list_size * sizeof(vertex_t *));

	/* fill the array with references to the 'real' sw in adj_list */
	for (i = 0; i < sw_list_size; i++)
		sw_list[i] = &(adj_list[i + 1]);

	/* sort the sw_list in descending order */
	sw_list_sort_by_num_hca(sw_list, sw_list_size);

	/* parse compute node guid file, if provided by the user */
	if (p_mgr->p_subn->opt.cn_guid_file) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
			"Parsing compute nodes from file %s\n",
			p_mgr->p_subn->opt.cn_guid_file);

		if (parse_node_map(p_mgr->p_subn->opt.cn_guid_file,
				   add_guid_to_map, &cn_tbl)) {
			OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR,
				"ERR AD33: Problem parsing compute node guid file\n");
			goto ERROR;
		}

		if (cl_is_qmap_empty(&cn_tbl))
			OSM_LOG(p_mgr->p_log, OSM_LOG_INFO,
				"WRN AD34: compute node guids file contains no valid guids\n");
		else
			cn_nodes_provided = TRUE;
	}

	/* parse I/O guid file, if provided by the user */
	if (p_mgr->p_subn->opt.io_guid_file) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
			"Parsing I/O nodes from file %s\n",
			p_mgr->p_subn->opt.io_guid_file);

		if (parse_node_map(p_mgr->p_subn->opt.io_guid_file,
				   add_guid_to_map, &io_tbl)) {
			OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR,
				"ERR AD35: Problem parsing I/O guid file\n");
			goto ERROR;
		}

		if (cl_is_qmap_empty(&io_tbl))
			OSM_LOG(p_mgr->p_log, OSM_LOG_INFO,
				"WRN AD36: I/O node guids file contains no valid guids\n");
		else
			io_nodes_provided = TRUE;
	}

	/* if we mix Hca/Tca/SP0 during the dijkstra routing, we might end up
	   in rare cases with a bad balancing for Hca<->Hca connections, i.e.
	   some inter-switch links get oversubscribed with paths;
	   therefore: add Hca ports first to ensure good Hca<->Hca balancing
	 */
	if (cn_nodes_provided) {
		for (i = 0; i < adj_list_size - 1; i++) {
			if (sw_list[i] && sw_list[i]->sw) {
				sw = (osm_switch_t *)(sw_list[i]->sw);
				add_sw_endports_to_order_list(sw, p_mgr,
							      &cn_tbl, TRUE);
			} else {
				OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR,
					"ERR AD30: corrupted sw_list array in dfsssp_do_dijkstra_routing\n");
				goto ERROR;
			}
		}
	}
	/* then: add Tca ports to ensure good Hca->Tca balancing and separate
	   paths towards I/O nodes on the same switch (if possible)
	 */
	if (io_nodes_provided) {
		for (i = 0; i < adj_list_size - 1; i++) {
			if (sw_list[i] && sw_list[i]->sw) {
				sw = (osm_switch_t *)(sw_list[i]->sw);
				add_sw_endports_to_order_list(sw, p_mgr,
							      &io_tbl, TRUE);
			} else {
				OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR,
					"ERR AD32: corrupted sw_list array in dfsssp_do_dijkstra_routing\n");
				goto ERROR;
			}
		}
	}
	/* then: add anything else, such as administration nodes, ... */
	if (cn_nodes_provided && io_nodes_provided) {
		cl_qmap_merge(&cn_tbl, &io_tbl);
	} else if (io_nodes_provided) {
		p_mixed_tbl = &io_tbl;
	}
	for (i = 0; i < adj_list_size - 1; i++) {
		if (sw_list[i] && sw_list[i]->sw) {
			sw = (osm_switch_t *)(sw_list[i]->sw);
			add_sw_endports_to_order_list(sw, p_mgr, p_mixed_tbl,
						      FALSE);
		} else {
			OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR,
				"ERR AD39: corrupted sw_list array in dfsssp_do_dijkstra_routing\n");
			goto ERROR;
		}
	}
	/* last: add SP0 afterwards which have lower priority for balancing */
	for (i = 0; i < sw_list_size; i++) {
		if (sw_list[i] && sw_list[i]->sw) {
			sw = (osm_switch_t *)(sw_list[i]->sw);
			guid = cl_ntoh64(osm_node_get_node_guid(sw->p_node));
			add_guid_to_order_list(guid, p_mgr);
		} else {
			OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR,
				"ERR AD31: corrupted sw_list array in dfsssp_do_dijkstra_routing\n");
			goto ERROR;
		}
	}

	/* the intermediate array lived long enough */
	free(sw_list);
	sw_list = NULL;
	/* same is true for the compute node and I/O guid map */
	destroy_guid_map(&cn_tbl);
	cn_nodes_provided = FALSE;
	destroy_guid_map(&io_tbl);
	io_nodes_provided = FALSE;

	/* do the routing for the each Hca in the subnet and each switch
	   in the subnet (to add the routes to base/enhanced SP0)
	 */
	qlist = &p_mgr->port_order_list;
	for (qlist_item = cl_qlist_head(qlist);
	     qlist_item != cl_qlist_end(qlist);
	     qlist_item = cl_qlist_next(qlist_item)) {
		port = (osm_port_t *)cl_item_obj(qlist_item, port, list_item);

		/* calculate shortest path with dijkstra from node to all switches/Hca */
		if (osm_node_get_type(port->p_node) == IB_NODE_TYPE_CA) {
			OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
				"Processing Hca with GUID 0x%" PRIx64 "\n",
				cl_ntoh64(osm_node_get_node_guid
					  (port->p_node)));
		} else if (osm_node_get_type(port->p_node) == IB_NODE_TYPE_SWITCH) {
			OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
				"Processing switch with GUID 0x%" PRIx64 "\n",
				cl_ntoh64(osm_node_get_node_guid
					  (port->p_node)));
		} else {
			/* we don't handle routers, in case they show up */
			continue;
		}

		/* distribute the LID range across the ports that can reach those LIDs
		   to have disjoint paths for one destination port with lmc>0;
		   for switches with bsp0: min=max; with esp0: max>min if lmc>0
		 */
		osm_port_get_lid_range_ho(port, &min_lid_ho,
					  &max_lid_ho);
		for (lid = min_lid_ho; lid <= max_lid_ho; lid++) {
			/* do dijkstra from this Hca/LID/SP0 to each switch */
			err =
			    dijkstra(p_mgr, adj_list, adj_list_size, port, lid);
			if (err)
				goto ERROR;
			if (OSM_LOG_IS_ACTIVE_V2(p_mgr->p_log, OSM_LOG_DEBUG))
				print_routes(p_mgr, adj_list, adj_list_size,
					     port);

			/* make an update for the linear forwarding tables of the switches */
			err =
			    update_lft(p_mgr, adj_list, adj_list_size, port, lid);
			if (err)
				goto ERROR;

			/* add weights for calculated routes to adjust the weights for the next cycle */
			update_weights(p_mgr, adj_list, adj_list_size);

			if (OSM_LOG_IS_ACTIVE_V2(p_mgr->p_log, OSM_LOG_DEBUG))
				dfsssp_print_graph(p_mgr, adj_list,
						   adj_list_size);
		}
	}

	/* try deadlock removal only for the dfsssp routing (not for the sssp case, which is a subset of the dfsssp algorithm) */
	if (dfsssp_ctx->routing_type == OSM_ROUTING_ENGINE_TYPE_DFSSSP) {
		/* remove potential deadlocks by assigning different virtual lanes to src/dest paths and balance the lanes */
		err = dfsssp_remove_deadlocks(dfsssp_ctx);
		if (err)
			goto ERROR;
	} else if (dfsssp_ctx->routing_type == OSM_ROUTING_ENGINE_TYPE_SSSP) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_INFO,
			"SSSP routing specified -> skipping deadlock removal thru dfsssp_remove_deadlocks(...)\n");
	} else {
		OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR,
			"ERR AD28: wrong routing engine specified in dfsssp_ctx\n");
		goto ERROR;
	}

	/* list not needed after the dijkstra steps and deadlock removal */
	cl_qlist_remove_all(&p_mgr->port_order_list);

	/* print the new_lft for each switch after routing is done */
	if (OSM_LOG_IS_ACTIVE_V2(p_mgr->p_log, OSM_LOG_DEBUG)) {
		for (item = cl_qmap_head(sw_tbl); item != cl_qmap_end(sw_tbl);
		     item = cl_qmap_next(item)) {
			sw = (osm_switch_t *) item;
			OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
				"Summary of the (new) LFT for switch 0x%" PRIx64
				" (%s):\n",
				cl_ntoh64(osm_node_get_node_guid(sw->p_node)),
				sw->p_node->print_desc);
			for (i = 0; i < sw->max_lid_ho + 1; i++)
				if (sw->new_lft[i] != OSM_NO_PATH) {
					OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
						"   for LID=%" PRIu32
						" use port=%" PRIu8 "\n", i,
						sw->new_lft[i]);
				}
		}
	}

	OSM_LOG_EXIT(p_mgr->p_log);
	return 0;

ERROR:
	if (!cl_is_qlist_empty(&p_mgr->port_order_list))
		cl_qlist_remove_all(&p_mgr->port_order_list);
	if (cn_nodes_provided)
		destroy_guid_map(&cn_tbl);
	if (io_nodes_provided)
		destroy_guid_map(&io_tbl);
	if (sw_list)
		free(sw_list);
	return -1;
}