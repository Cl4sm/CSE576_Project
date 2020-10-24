static int rank_leaf_switches(IN ftree_fabric_t * p_ftree,
			      IN ftree_hca_t * p_hca,
			      IN cl_list_t * p_ranking_bfs_list)
{
	ftree_sw_t *p_sw;
	osm_node_t *p_osm_node = p_hca->p_osm_node;
	osm_node_t *p_remote_osm_node;
	osm_physp_t *p_osm_port;
	static uint8_t i = 0;
	int res = 0;

	OSM_LOG_ENTER(&p_ftree->p_osm->log);

	for (i = 0; i < osm_node_get_num_physp(p_osm_node); i++) {
		p_osm_port = osm_node_get_physp_ptr(p_osm_node, i);
		if (!p_osm_port || !osm_link_is_healthy(p_osm_port))
			continue;

		p_remote_osm_node =
		    osm_node_get_remote_node(p_osm_node, i, NULL);
		if (!p_remote_osm_node)
			continue;

		switch (osm_node_get_type(p_remote_osm_node)) {
		case IB_NODE_TYPE_CA:
			/* HCA connected directly to another HCA - not FatTree */
			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_ERROR,
				"ERR AB0F: "
				"CA conected directly to another CA: " "0x%016"
				PRIx64 " <---> 0x%016" PRIx64 "\n",
				hca_get_guid_ho(p_hca),
				cl_ntoh64(osm_node_get_node_guid
					  (p_remote_osm_node)));
			res = -1;
			goto Exit;

		case IB_NODE_TYPE_ROUTER:
			/* leaving this port - proceeding to the next one */
			continue;

		case IB_NODE_TYPE_SWITCH:
			/* continue with this port */
			break;

		default:
			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_ERROR,
				"ERR AB10: Node GUID 0x%016" PRIx64
				" - Unknown node type: %s\n",
				cl_ntoh64(osm_node_get_node_guid
					  (p_remote_osm_node)),
				ib_get_node_type_str(osm_node_get_type
						     (p_remote_osm_node)));
			res = -1;
			goto Exit;
		}

		/* remote node is switch */

		p_sw = fabric_get_sw_by_guid(p_ftree,
					     osm_node_get_node_guid
					     (p_osm_port->p_remote_physp->
					      p_node));
		CL_ASSERT(p_sw);

		/* if needed, rank the remote switch and add it to the BFS list */

		if (!sw_update_rank(p_sw, 0))
			continue;
		OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
			"Marking rank of switch that is directly connected to CA:\n"
			"                                            - CA guid    : 0x%016"
			PRIx64 "\n"
			"                                            - Switch guid: 0x%016"
			PRIx64 "\n"
			"                                            - Switch LID : %u\n",
			hca_get_guid_ho(p_hca),
			sw_get_guid_ho(p_sw), p_sw->base_lid);
		cl_list_insert_tail(p_ranking_bfs_list, p_sw);
	}

Exit:
	OSM_LOG_EXIT(&p_ftree->p_osm->log);
	return res;
}				/* rank_leaf_switches() */