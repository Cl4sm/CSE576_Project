static int lash_core(lash_t * p_lash)
{
	osm_log_t *p_log = &p_lash->p_osm->log;
	unsigned num_switches = p_lash->num_switches;
	switch_t **switches = p_lash->switches;
	unsigned lanes_needed = 1;
	unsigned int i, j, k, dest_switch = 0;
	reachable_dest_t *dests, *idest;
	int cycle_found = 0;
	unsigned v_lane;
	int stop = 0, output_link, i_next_switch;
	int output_link2, i_next_switch2;
	int cycle_found2 = 0;
	int status = -1;
	int *switch_bitmap = NULL;	/* Bitmap to check if we have processed this pair */
	unsigned start_vl = p_lash->p_osm->subn.opt.lash_start_vl;

	OSM_LOG_ENTER(p_log);

	if (p_lash->p_osm->subn.opt.do_mesh_analysis && osm_do_mesh_analysis(p_lash)) {
		OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 4D05: Mesh analysis failed\n");
		goto Exit;
	}

	for (i = 0; i < num_switches; i++) {

		shortest_path(p_lash, i);
		if (generate_routing_func_for_mst(p_lash, i, &dests)) {
			OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 4D06: "
				"generate_routing_func_for_mst failed\n");
			goto Exit;
		}

		idest = dests;
		while (idest != NULL) {
			dests = dests->next;
			free(idest);
			idest = dests;
		}

		for (j = 0; j < num_switches; j++) {
			switches[j]->used_channels = 0;
			switches[j]->q_state = UNQUEUED;
		}
	}

	switch_bitmap = calloc(num_switches * num_switches, sizeof(int));
	if (!switch_bitmap) {
		OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 4D04: "
			"Failed allocating switch_bitmap - out of memory\n");
		goto Exit;
	}

	for (i = 0; i < num_switches; i++) {
		for (dest_switch = 0; dest_switch < num_switches; dest_switch++)
			if (dest_switch != i && switch_bitmap[i * num_switches + dest_switch] == 0) {
				v_lane = 0;
				stop = 0;
				while (v_lane < lanes_needed && stop == 0) {
					if (generate_cdg_for_sp(p_lash, i, dest_switch, v_lane) ||
					    generate_cdg_for_sp(p_lash, dest_switch, i, v_lane)) {
						OSM_LOG(p_log, OSM_LOG_ERROR,
							"ERR 4D07: generate_cdg_for_sp failed\n");
						goto Exit;
					}

					output_link =
					    switches[i]->routing_table[dest_switch].out_link;
					output_link2 =
					    switches[dest_switch]->routing_table[i].out_link;

					i_next_switch = get_next_switch(p_lash, i, output_link);
					i_next_switch2 = get_next_switch(p_lash, dest_switch, output_link2);

					CL_ASSERT(p_lash->
						  cdg_vertex_matrix[v_lane][i][i_next_switch] !=
						  NULL);
					CL_ASSERT(p_lash->
						  cdg_vertex_matrix[v_lane][dest_switch]
						  [i_next_switch2] != NULL);

					cycle_found =
					    cycle_exists(p_lash->
							 cdg_vertex_matrix[v_lane][i]
							 [i_next_switch], NULL, NULL, 1);
					cycle_found2 =
					    cycle_exists(p_lash->
							 cdg_vertex_matrix[v_lane][dest_switch]
							 [i_next_switch2], NULL, NULL, 1);

					for (j = 0; j < num_switches; j++)
						for (k = 0; k < num_switches; k++)
							if (p_lash->
							    cdg_vertex_matrix[v_lane][j][k] !=
							    NULL) {
								p_lash->
								    cdg_vertex_matrix[v_lane][j]
								    [k]->visiting_number = 0;
								p_lash->
								    cdg_vertex_matrix[v_lane][j]
								    [k]->seen = 0;
							}

					if (cycle_found == 1 || cycle_found2 == 1) {
						remove_temp_depend_for_sp(p_lash, i, dest_switch,
									  v_lane);
						remove_temp_depend_for_sp(p_lash, dest_switch, i,
									  v_lane);
						v_lane++;
					} else {
						set_temp_depend_to_permanent_for_sp(p_lash, i,
										    dest_switch,
										    v_lane);
						set_temp_depend_to_permanent_for_sp(p_lash,
										    dest_switch, i,
										    v_lane);
						stop = 1;
						p_lash->num_mst_in_lane[v_lane]++;
						p_lash->num_mst_in_lane[v_lane]++;
					}
				}

				switches[i]->routing_table[dest_switch].lane = v_lane + start_vl;
				switches[dest_switch]->routing_table[i].lane = v_lane + start_vl;

				if (cycle_found == 1 || cycle_found2 == 1) {
					if (++lanes_needed > p_lash->vl_min)
						goto Error_Not_Enough_Lanes;

					if (generate_cdg_for_sp(p_lash, i, dest_switch, v_lane) ||
					    generate_cdg_for_sp(p_lash, dest_switch, i, v_lane)) {
						OSM_LOG(p_log, OSM_LOG_ERROR,
							"ERR 4D08: generate_cdg_for_sp failed\n");
						goto Exit;
					}

					set_temp_depend_to_permanent_for_sp(p_lash, i, dest_switch,
									    v_lane);
					set_temp_depend_to_permanent_for_sp(p_lash, dest_switch, i,
									    v_lane);

					p_lash->num_mst_in_lane[v_lane]++;
					p_lash->num_mst_in_lane[v_lane]++;
				}
				p_lash->virtual_location[i][dest_switch][v_lane] = 1;
				p_lash->virtual_location[dest_switch][i][v_lane] = 1;

				switch_bitmap[i * num_switches + dest_switch] = 1;
				switch_bitmap[dest_switch * num_switches + i] = 1;
			}
	}

	for (i = 0; i < lanes_needed; i++)
		OSM_LOG(p_log, OSM_LOG_INFO, "Lanes in layer %d: %d\n",
			i, p_lash->num_mst_in_lane[i]);

	OSM_LOG(p_log, OSM_LOG_INFO,
		"Lanes needed: %d, Balancing\n", lanes_needed);

	if (balance_virtual_lanes(p_lash, lanes_needed)) {
		OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 4D09: Balancing failed\n");
		goto Exit;
	}

	for (i = 0; i < lanes_needed; i++)
		OSM_LOG(p_log, OSM_LOG_INFO, "Lanes in layer %d: %d\n",
			i, p_lash->num_mst_in_lane[i]);

	status = 0;
	goto Exit;

Error_Not_Enough_Lanes:
	OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 4D02: "
		"Lane requirements (%d) exceed available lanes (%d)"
		" with starting lane (%d)\n",
		lanes_needed, p_lash->vl_min, start_vl);
Exit:
	if (switch_bitmap)
		free(switch_bitmap);
	OSM_LOG_EXIT(p_log);
	return status;
}