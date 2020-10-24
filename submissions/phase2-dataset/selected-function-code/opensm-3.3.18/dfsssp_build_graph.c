static int dfsssp_build_graph(void *context)
{
	dfsssp_context_t *dfsssp_ctx = (dfsssp_context_t *) context;
	osm_ucast_mgr_t *p_mgr = (osm_ucast_mgr_t *) (dfsssp_ctx->p_mgr);

	cl_qmap_t *port_tbl = &p_mgr->p_subn->port_guid_tbl;	/* 1 management port per switch + 1 or 2 ports for each Hca */
	osm_port_t *p_port = NULL;
	cl_qmap_t *sw_tbl = &p_mgr->p_subn->sw_guid_tbl;
	cl_map_item_t *item = NULL;
	osm_switch_t *sw = NULL;
	osm_node_t *remote_node = NULL;
	uint8_t port = 0, remote_port = 0;
	uint32_t i = 0, j = 0;
	uint64_t total_num_hca = 0;
	vertex_t *adj_list = NULL;
	osm_physp_t *p_physp = NULL;
	link_t *link = NULL, *head = NULL;
	uint32_t num_sw = 0, adj_list_size = 0;
	uint8_t lmc = 0;

	OSM_LOG_ENTER(p_mgr->p_log);
	OSM_LOG(p_mgr->p_log, OSM_LOG_VERBOSE,
		"Building graph for df-/sssp routing\n");

	/* if this pointer isn't NULL, this is a reroute step;
	   old context will be destroyed (adj_list and srcdest2vl_table)
	 */
	if (dfsssp_ctx->adj_list)
		dfsssp_context_destroy(context);

	num_sw = cl_qmap_count(sw_tbl);
	adj_list_size = num_sw + 1;
	/* allocate an adjazenz list (array), 0. element is reserved for the source (Hca) in the routing algo, others are switches */
	adj_list = (vertex_t *) malloc(adj_list_size * sizeof(vertex_t));
	if (!adj_list) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR,
			"ERR AD02: cannot allocate memory for adj_list\n");
		goto ERROR;
	}
	for (i = 0; i < adj_list_size; i++)
		set_default_vertex(&adj_list[i]);

	dfsssp_ctx->adj_list = adj_list;
	dfsssp_ctx->adj_list_size = adj_list_size;

	/* count the total number of Hca / LIDs (for lmc>0) in the fabric;
	   even include base/enhanced switch port 0; base SP0 will have lmc=0
	 */
	for (item = cl_qmap_head(port_tbl); item != cl_qmap_end(port_tbl);
	     item = cl_qmap_next(item)) {
		p_port = (osm_port_t *) item;
		if (osm_node_get_type(p_port->p_node) == IB_NODE_TYPE_CA ||
		    osm_node_get_type(p_port->p_node) == IB_NODE_TYPE_SWITCH) {
			lmc = osm_port_get_lmc(p_port);
			total_num_hca += (1 << lmc);
		}
	}

	i = 1;			/* fill adj_list -> start with index 1 */
	for (item = cl_qmap_head(sw_tbl); item != cl_qmap_end(sw_tbl);
	     item = cl_qmap_next(item), i++) {
		sw = (osm_switch_t *) item;
		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
			"Processing switch with GUID 0x%" PRIx64 "\n",
			cl_ntoh64(osm_node_get_node_guid(sw->p_node)));

		adj_list[i].guid =
		    cl_ntoh64(osm_node_get_node_guid(sw->p_node));
		adj_list[i].lid =
		    cl_ntoh16(osm_node_get_base_lid(sw->p_node, 0));
		adj_list[i].sw = sw;

		link = (link_t *) malloc(sizeof(link_t));
		if (!link) {
			OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR,
				"ERR AD03: cannot allocate memory for a link\n");
			dfsssp_context_destroy(context);
			goto ERROR;
		}
		head = link;
		head->next = NULL;

		/* add SP0 to number of CA conneted to a switch */
		lmc = osm_node_get_lmc(sw->p_node, 0);
		adj_list[i].num_hca += (1 << lmc);

		/* iterate over all ports in the switch, start with port 1 (port 0 is a management port) */
		for (port = 1; port < sw->num_ports; port++) {
			/* get the node behind the port */
			remote_node =
			    osm_node_get_remote_node(sw->p_node, port,
						     &remote_port);
			/* if there is no remote node on this port or it's the same switch -> try next port */
			if (!remote_node || remote_node->sw == sw)
				continue;
			/* make sure the link is healthy */
			p_physp = osm_node_get_physp_ptr(sw->p_node, port);
			if (!p_physp || !osm_link_is_healthy(p_physp))
				continue;
			/* if there is a Hca connected -> count and cycle */
			if (!remote_node->sw) {
				lmc = osm_node_get_lmc(remote_node, (uint32_t)remote_port);
				adj_list[i].num_hca += (1 << lmc);
				continue;
			}
			OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
				"Node 0x%" PRIx64 ", remote node 0x%" PRIx64
				", port %" PRIu8 ", remote port %" PRIu8 "\n",
				cl_ntoh64(osm_node_get_node_guid(sw->p_node)),
				cl_ntoh64(osm_node_get_node_guid(remote_node)),
				port, remote_port);

			link->next = (link_t *) malloc(sizeof(link_t));
			if (!link->next) {
				OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR,
					"ERR AD08: cannot allocate memory for a link\n");
				dfsssp_context_destroy(context);
				while (head) {
					link = head;
					head = head->next;
					free(link);
				}
				goto ERROR;
			}
			link = link->next;
			set_default_link(link);
			link->guid =
			    cl_ntoh64(osm_node_get_node_guid(remote_node));
			link->from = i;
			link->from_port = port;
			link->to_port = remote_port;
			link->weight = total_num_hca * total_num_hca;	/* initialize with P^2 to force shortest paths */
		}

		adj_list[i].links = head->next;
		free(head);
	}
	/* connect the links with it's second adjacent node in the list */
	for (i = 1; i < adj_list_size; i++) {
		link = adj_list[i].links;
		while (link) {
			for (j = 1; j < adj_list_size; j++) {
				if (link->guid == adj_list[j].guid) {
					link->to = j;
					break;
				}
			}
			link = link->next;
		}
	}
	/* print the discovered graph */
	if (OSM_LOG_IS_ACTIVE_V2(p_mgr->p_log, OSM_LOG_DEBUG))
		dfsssp_print_graph(p_mgr, adj_list, adj_list_size);

	OSM_LOG_EXIT(p_mgr->p_log);
	return 0;

ERROR:
	return -1;
}