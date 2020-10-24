static int fabric_populate_nodes(IN ftree_fabric_t * p_ftree)
{
	osm_node_t *p_osm_node;
	osm_node_t *p_next_osm_node;

	OSM_LOG_ENTER(&p_ftree->p_osm->log);

	p_next_osm_node =
	    (osm_node_t *) cl_qmap_head(&p_ftree->p_osm->subn.node_guid_tbl);
	while (p_next_osm_node !=
	       (osm_node_t *) cl_qmap_end(&p_ftree->p_osm->
					  subn.node_guid_tbl)) {
		p_osm_node = p_next_osm_node;
		p_next_osm_node =
		    (osm_node_t *) cl_qmap_next(&p_osm_node->map_item);
		switch (osm_node_get_type(p_osm_node)) {
		case IB_NODE_TYPE_CA:
			fabric_add_hca(p_ftree, p_osm_node);
			break;
		case IB_NODE_TYPE_ROUTER:
			break;
		case IB_NODE_TYPE_SWITCH:
			fabric_add_sw(p_ftree, p_osm_node->sw);
			break;
		default:
			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_ERROR,
				"ERR AB0E: " "Node GUID 0x%016" PRIx64
				" - Unknown node type: %s\n",
				cl_ntoh64(osm_node_get_node_guid(p_osm_node)),
				ib_get_node_type_str(osm_node_get_type
						     (p_osm_node)));
			OSM_LOG_EXIT(&p_ftree->p_osm->log);
			return -1;
		}
	}

	OSM_LOG_EXIT(&p_ftree->p_osm->log);
	return 0;
}				/* fabric_populate_nodes() */