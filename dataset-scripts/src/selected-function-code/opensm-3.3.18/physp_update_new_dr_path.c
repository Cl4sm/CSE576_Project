static void physp_update_new_dr_path(IN osm_physp_t const *p_dest_physp,
				     IN cl_map_t * p_visited_map,
				     IN osm_bind_handle_t * h_bind)
{
	cl_list_t tmpPortsList;
	osm_physp_t *p_physp, *p_src_physp = NULL;
	uint8_t path_array[IB_SUBNET_PATH_HOPS_MAX];
	uint8_t i = 0;
	osm_dr_path_t *p_dr_path;

	cl_list_construct(&tmpPortsList);
	cl_list_init(&tmpPortsList, 10);

	cl_list_insert_head(&tmpPortsList, p_dest_physp);
	/* get the output port where we need to come from */
	p_physp = (osm_physp_t *) cl_map_get(p_visited_map,
					     ptr_to_key(p_dest_physp));
	while (p_physp != NULL) {
		cl_list_insert_head(&tmpPortsList, p_physp);
		/* get the input port through where we reached the output port */
		p_src_physp = p_physp;
		p_physp = (osm_physp_t *) cl_map_get(p_visited_map,
						     ptr_to_key(p_physp));
		/* if we reached a null p_physp - this means we are at the begining
		   of the path. Break. */
		if (p_physp == NULL)
			break;
		/* get the output port */
		p_physp = (osm_physp_t *) cl_map_get(p_visited_map,
						     ptr_to_key(p_physp));
	}

	memset(path_array, 0, sizeof(path_array));
	p_physp = (osm_physp_t *) cl_list_remove_head(&tmpPortsList);
	while (p_physp != NULL) {
		i++;
		path_array[i] = p_physp->port_num;
		p_physp = (osm_physp_t *) cl_list_remove_head(&tmpPortsList);
	}
	if (p_src_physp) {
		p_dr_path = osm_physp_get_dr_path_ptr(p_src_physp);
		osm_dr_path_init(p_dr_path, i, path_array);
	}

	cl_list_destroy(&tmpPortsList);
}