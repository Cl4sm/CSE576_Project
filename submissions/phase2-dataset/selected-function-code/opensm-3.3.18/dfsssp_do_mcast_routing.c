static ib_api_status_t dfsssp_do_mcast_routing(void * context,
					       osm_mgrp_box_t * mbox)
{
	dfsssp_context_t *dfsssp_ctx = (dfsssp_context_t *) context;
	osm_ucast_mgr_t *p_mgr = (osm_ucast_mgr_t *) dfsssp_ctx->p_mgr;
	osm_sm_t *sm = (osm_sm_t *) p_mgr->sm;
	vertex_t *adj_list = (vertex_t *) dfsssp_ctx->adj_list;
	uint32_t adj_list_size = dfsssp_ctx->adj_list_size;
	cl_qlist_t mcastgrp_port_list;
	cl_qmap_t mcastgrp_port_map;
	osm_switch_t *root_sw = NULL;
	osm_port_t *port = NULL;
	ib_net16_t lid = 0;
	uint32_t err = 0, num_ports = 0;
	ib_api_status_t status = IB_SUCCESS;

	OSM_LOG_ENTER(sm->p_log);

	/* create a map and a list of all ports which are member in the mcast
	   group; map for searching elements and list for iteration
	 */
	if (osm_mcast_make_port_list_and_map(&mcastgrp_port_list,
					     &mcastgrp_port_map, mbox)) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR AD50: "
                        "Insufficient memory to make port list\n");
                status = IB_ERROR;
                goto Exit;
        }

	num_ports = cl_qlist_count(&mcastgrp_port_list);
	if (num_ports < 2) {
		OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
			"MLID 0x%X has %u members - nothing to do\n",
			mbox->mlid, num_ports);
		goto Exit;
	}

	/* find the root switch for the spanning tree, which has the smallest
	   hops count to all LIDs in the mcast group
	 */
	root_sw = osm_mcast_mgr_find_root_switch(sm, &mcastgrp_port_list);
	if (!root_sw) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR AD51: "
			"Unable to locate a suitable switch for group 0x%X\n",
			mbox->mlid);
		status = IB_ERROR;
		goto Exit;
	}

	/* a) start one dijkstra step from the root switch to generate a
	   spanning tree
	   b) this might be a bit of an overkill to span the whole
	   network, if there are only a few ports in the mcast group, but
	   its only one dijkstra step for each mcast group and we did many
	   steps before in the ucast routing for each LID in the subnet;
	   c) we can use the subnet structure from the ucast routing, and
	   don't even have to reset the link weights (=> therefore the mcast
	   spanning tree will use less 'growded' links in the network)
	   d) the mcast dfsssp algorithm will not change the link weights
	 */
	lid = osm_node_get_base_lid(root_sw->p_node, 0);
	port = osm_get_port_by_lid(sm->p_subn, lid);
	err = dijkstra(p_mgr, adj_list, adj_list_size, port, lid);
	if (err) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR AD52: "
			"Dijkstra step for mcast failed for group 0x%X\n",
			mbox->mlid);
		status = IB_ERROR;
		goto Exit;
	}

	/* set mcast group membership again for update_mcft
	   (unfortunately: osm_mcast_mgr_find_root_switch resets it)
	 */
	update_mgrp_membership(&mcastgrp_port_list);

	/* update the mcast forwarding tables of the switches */
	err = update_mcft(sm, adj_list, adj_list_size, mbox->mlid,
			  &mcastgrp_port_map, root_sw);
	if (err) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR AD53: "
			"Update of mcast forwarding tables failed for group 0x%X\n",
			mbox->mlid);
		status = IB_ERROR;
		goto Exit;
	}

Exit:
	reset_mgrp_membership(adj_list, adj_list_size);
	osm_mcast_drop_port_list(&mcastgrp_port_list);
	OSM_LOG_EXIT(sm->p_log);
	return status;
}