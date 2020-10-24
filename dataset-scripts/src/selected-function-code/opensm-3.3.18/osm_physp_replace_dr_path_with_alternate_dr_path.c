void osm_physp_replace_dr_path_with_alternate_dr_path(IN osm_log_t * p_log,
						      IN osm_subn_t const
						      *p_subn, IN osm_physp_t const
						      *p_dest_physp,
						      IN osm_bind_handle_t *
						      h_bind)
{
	cl_map_t physp_map;
	cl_map_t visited_map;
	osm_dr_path_t *p_dr_path;
	cl_list_t *p_currPortsList;
	cl_list_t *p_nextPortsList;
	osm_port_t *p_port;
	osm_physp_t *p_physp, *p_remote_physp;
	ib_net64_t port_guid;
	boolean_t next_list_is_full = TRUE, reached_dest = FALSE;
	uint8_t num_ports, port_num;

	p_nextPortsList = (cl_list_t *) malloc(sizeof(cl_list_t));
	if (!p_nextPortsList)
		return;

	/*
	   initialize the map of all port participating in current dr path
	   not including first and last switches
	 */
	cl_map_construct(&physp_map);
	cl_map_init(&physp_map, 4);
	cl_map_construct(&visited_map);
	cl_map_init(&visited_map, 4);
	p_dr_path = osm_physp_get_dr_path_ptr(p_dest_physp);
	physp_get_dr_physp_set(p_log, p_subn, p_dr_path, &physp_map);

	/*
	   BFS from OSM port until we find the target physp but avoid
	   going through mapped ports
	 */
	cl_list_construct(p_nextPortsList);
	cl_list_init(p_nextPortsList, 10);

	port_guid = p_subn->sm_port_guid;

	CL_ASSERT(port_guid);

	p_port = osm_get_port_by_guid(p_subn, port_guid);
	if (!p_port) {
		OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 4105: No SM port object\n");
		goto Exit;
	}

	/*
	   HACK: We are assuming SM is running on HCA, so when getting the default
	   port we'll get the port connected to the rest of the subnet. If SM is
	   running on SWITCH - we should try to get a dr path from all switch ports.
	 */
	p_physp = p_port->p_physp;

	CL_ASSERT(p_physp);

	cl_list_insert_tail(p_nextPortsList, p_physp);

	while (next_list_is_full == TRUE) {
		next_list_is_full = FALSE;
		p_currPortsList = p_nextPortsList;
		p_nextPortsList = (cl_list_t *) malloc(sizeof(cl_list_t));
		if (!p_nextPortsList) {
			p_nextPortsList = p_currPortsList;
			goto Exit;
		}
		cl_list_construct(p_nextPortsList);
		cl_list_init(p_nextPortsList, 10);
		p_physp = (osm_physp_t *) cl_list_remove_head(p_currPortsList);
		while (p_physp != NULL) {
			/* If we are in a switch - need to go out through all
			   the other physical ports of the switch */
			num_ports = osm_node_get_num_physp(p_physp->p_node);

			for (port_num = 1; port_num < num_ports; port_num++) {
				if (osm_node_get_type(p_physp->p_node) ==
				    IB_NODE_TYPE_SWITCH)
					p_remote_physp =
					    osm_node_get_physp_ptr(p_physp->
								   p_node,
								   port_num);
				else
					/* this is HCA or router - the remote port is just the port connected
					   on the other side */
					p_remote_physp =
					    p_physp->p_remote_physp;

				/*
				   make sure that all of the following occurred:
				   1. The port isn't NULL
				   2. This is not the port we came from
				   3. The port is not in the physp_map
				   4. This port haven't been visited before
				 */
				if (p_remote_physp &&
				    p_remote_physp != p_physp &&
				    cl_map_get(&physp_map,
					       ptr_to_key(p_remote_physp))
				    == NULL
				    && cl_map_get(&visited_map,
						  ptr_to_key
						  (p_remote_physp)) == NULL) {
					/* Insert the port into the visited_map, and save its source port */
					cl_map_insert(&visited_map,
						      ptr_to_key
						      (p_remote_physp),
						      p_physp);

					/* Is this the p_dest_physp? */
					if (p_remote_physp == p_dest_physp) {
						/* update the new dr path */
						physp_update_new_dr_path
						    (p_dest_physp, &visited_map,
						     h_bind);
						reached_dest = TRUE;
						break;
					}

					/* add the p_remote_physp to the nextPortsList */
					cl_list_insert_tail(p_nextPortsList,
							    p_remote_physp);
					next_list_is_full = TRUE;
				}
			}

			p_physp = (osm_physp_t *)
			    cl_list_remove_head(p_currPortsList);
			if (reached_dest == TRUE) {
				/* free the rest of the currPortsList */
				while (p_physp != NULL)
					p_physp = (osm_physp_t *)
					    cl_list_remove_head
					    (p_currPortsList);
				/* free the nextPortsList, if items were added to it */
				p_physp = (osm_physp_t *)
				    cl_list_remove_head(p_nextPortsList);
				while (p_physp != NULL)
					p_physp = (osm_physp_t *)
					    cl_list_remove_head
					    (p_nextPortsList);
				next_list_is_full = FALSE;
			}
		}
		cl_list_destroy(p_currPortsList);
		free(p_currPortsList);
	}

	/* cleanup */
Exit:
	cl_list_destroy(p_nextPortsList);
	free(p_nextPortsList);
	cl_map_destroy(&physp_map);
	cl_map_destroy(&visited_map);
}