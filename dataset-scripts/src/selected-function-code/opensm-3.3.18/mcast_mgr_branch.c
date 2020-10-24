static osm_mtree_node_t *mcast_mgr_branch(osm_sm_t * sm, uint16_t mlid_ho,
					  osm_switch_t * p_sw,
					  cl_qlist_t * p_list, uint8_t depth,
					  uint8_t upstream_port,
					  uint8_t * p_max_depth)
{
	uint8_t max_children;
	osm_mtree_node_t *p_mtn = NULL;
	cl_qlist_t *list_array = NULL;
	uint8_t i;
	ib_net64_t node_guid;
	osm_mcast_work_obj_t *p_wobj;
	cl_qlist_t *p_port_list;
	size_t count;
	osm_mcast_tbl_t *p_tbl;

	OSM_LOG_ENTER(sm->p_log);

	CL_ASSERT(p_sw);
	CL_ASSERT(p_list);
	CL_ASSERT(p_max_depth);

	node_guid = osm_node_get_node_guid(p_sw->p_node);

	OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
		"Routing MLID 0x%X through switch 0x%" PRIx64
		" %s, %u nodes at depth %u\n",
		mlid_ho, cl_ntoh64(node_guid), p_sw->p_node->print_desc,
		cl_qlist_count(p_list), depth);

	CL_ASSERT(cl_qlist_count(p_list) > 0);

	depth++;

	if (depth >= 64) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0A21: "
			"Maximal hops number is reached for MLID 0x%x."
			" Break processing\n", mlid_ho);
		mcast_mgr_purge_list(sm, p_list);
		goto Exit;
	}

	if (depth > *p_max_depth) {
		CL_ASSERT(depth == *p_max_depth + 1);
		*p_max_depth = depth;
	}

	if (osm_switch_supports_mcast(p_sw) == FALSE) {
		/*
		   This switch doesn't do multicast.  Clean-up.
		 */
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0A14: "
			"Switch 0x%" PRIx64 " %s does not support multicast\n",
			cl_ntoh64(node_guid), p_sw->p_node->print_desc);

		/*
		   Deallocate all the work objects on this branch of the tree.
		 */
		mcast_mgr_purge_list(sm, p_list);
		goto Exit;
	}

	p_mtn = osm_mtree_node_new(p_sw);
	if (p_mtn == NULL) {
		/*
		   We are unable to continue routing down this
		   leg of the tree.  Clean-up.
		 */
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0A15: "
			"Insufficient memory to build multicast tree\n");

		/*
		   Deallocate all the work objects on this branch of the tree.
		 */
		mcast_mgr_purge_list(sm, p_list);
		goto Exit;
	}

	max_children = osm_mtree_node_get_max_children(p_mtn);

	CL_ASSERT(max_children > 1);

	/*
	   Prepare an empty list for each port in the switch.
	   TO DO - this list array could probably be moved
	   inside the switch element to save on malloc thrashing.
	 */
	list_array = malloc(sizeof(cl_qlist_t) * max_children);
	if (list_array == NULL) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0A16: "
			"Unable to allocate list array\n");
		mcast_mgr_purge_list(sm, p_list);
		osm_mtree_destroy(p_mtn);
		p_mtn = NULL;
		goto Exit;
	}

	memset(list_array, 0, sizeof(cl_qlist_t) * max_children);

	for (i = 0; i < max_children; i++)
		cl_qlist_init(&list_array[i]);

	mcast_mgr_subdivide(sm, mlid_ho, p_sw, p_list, list_array, max_children);

	p_tbl = osm_switch_get_mcast_tbl_ptr(p_sw);

	/*
	   Add the upstream port to the forwarding table unless
	   we're at the root of the spanning tree.
	 */
	if (depth > 1) {
		OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
			"Adding upstream port %u\n", upstream_port);

		CL_ASSERT(upstream_port);
		osm_mcast_tbl_set(p_tbl, mlid_ho, upstream_port);
	}

	/*
	   For each port that was allocated some routes,
	   recurse into this function to continue building the tree
	   if the node on the other end of that port is another switch.
	   Otherwise, the node is an endpoint, and we've found a leaf
	   of the tree.  Mark leaves with our special pointer value.
	 */

	for (i = 0; i < max_children; i++) {
		const osm_physp_t *p_physp;
		const osm_physp_t *p_remote_physp;
		osm_node_t *p_node;
		const osm_node_t *p_remote_node;

		p_port_list = &list_array[i];

		count = cl_qlist_count(p_port_list);

		/*
		   There should be no children routed through the upstream port!
		 */
		CL_ASSERT(upstream_port == 0 || i != upstream_port ||
			  (i == upstream_port && count == 0));

		if (count == 0)
			continue;	/* No routes down this port. */

		OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
			"Routing %zu destinations via switch port %u\n",
			count, i);

		/*
		   This port routes frames for this mcast group.  Therefore,
		   set the appropriate bit in the multicast forwarding
		   table for this switch.
		 */
		osm_mcast_tbl_set(p_tbl, mlid_ho, i);
		if (i == 0) {
			/* This means we are adding the switch to the MC group.
			   We do not need to continue looking at the remote
			   port, just needed to add the port to the table */
			CL_ASSERT(count == 1);

			p_wobj = (osm_mcast_work_obj_t *)
			    cl_qlist_remove_head(p_port_list);
			mcast_work_obj_delete(p_wobj);
			continue;
		}

		p_node = p_sw->p_node;
		p_remote_node = osm_node_get_remote_node(p_node, i, NULL);
		if (!p_remote_node)
			continue;

		if (osm_node_get_type(p_remote_node) == IB_NODE_TYPE_SWITCH) {
			/*
			   Acquire a pointer to the remote switch then recurse.
			 */
			CL_ASSERT(p_remote_node->sw);

			p_physp = osm_node_get_physp_ptr(p_node, i);
			CL_ASSERT(p_physp);

			p_remote_physp = osm_physp_get_remote(p_physp);
			CL_ASSERT(p_remote_physp);

			p_mtn->child_array[i] =
			    mcast_mgr_branch(sm, mlid_ho, p_remote_node->sw,
					     p_port_list, depth,
					     osm_physp_get_port_num
					     (p_remote_physp), p_max_depth);
		} else {
			/*
			   The neighbor node is not a switch, so this
			   must be a leaf.
			 */
			CL_ASSERT(count == 1);

			p_mtn->child_array[i] = OSM_MTREE_LEAF;
			p_wobj = (osm_mcast_work_obj_t *)
			    cl_qlist_remove_head(p_port_list);

			CL_ASSERT(cl_is_qlist_empty(p_port_list));

			OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
				"Found leaf for port 0x%016" PRIx64
				" on switch port %u\n",
				cl_ntoh64(osm_port_get_guid(p_wobj->p_port)),
				i);
			mcast_work_obj_delete(p_wobj);
		}
	}

	free(list_array);
Exit:
	OSM_LOG_EXIT(sm->p_log);
	return p_mtn;
}