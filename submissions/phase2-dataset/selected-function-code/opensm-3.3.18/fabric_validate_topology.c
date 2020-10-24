static boolean_t fabric_validate_topology(IN ftree_fabric_t * p_ftree)
{
	ftree_port_group_t *p_group;
	ftree_port_group_t *p_ref_group;
	ftree_sw_t *p_sw;
	ftree_sw_t *p_next_sw;
	ftree_sw_t **reference_sw_arr;
	uint16_t tree_rank = fabric_get_rank(p_ftree);
	boolean_t res = TRUE;
	uint8_t i;

	OSM_LOG_ENTER(&p_ftree->p_osm->log);

	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_VERBOSE,
		"Validating fabric topology\n");

	reference_sw_arr =
	    (ftree_sw_t **) malloc(tree_rank * sizeof(ftree_sw_t *));
	if (reference_sw_arr == NULL) {
		osm_log_v2(&p_ftree->p_osm->log, OSM_LOG_SYS, FILE_ID,
			   "Fat-tree routing: Memory allocation failed\n");
		return FALSE;
	}
	memset(reference_sw_arr, 0, tree_rank * sizeof(ftree_sw_t *));

	p_next_sw = (ftree_sw_t *) cl_qmap_head(&p_ftree->sw_tbl);
	while (res && p_next_sw != (ftree_sw_t *) cl_qmap_end(&p_ftree->sw_tbl)) {
		p_sw = p_next_sw;
		p_next_sw = (ftree_sw_t *) cl_qmap_next(&p_sw->map_item);

		if (!reference_sw_arr[p_sw->rank])
			/* This is the first switch in the current level that
			   we're checking - use it as a reference */
			reference_sw_arr[p_sw->rank] = p_sw;
		else {
			/* compare this switch properties to the reference switch */

			if (reference_sw_arr[p_sw->rank]->up_port_groups_num !=
			    p_sw->up_port_groups_num) {
				OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_ERROR,
					"ERR AB09: Different number of upward port groups on switches:\n"
					"       GUID 0x%016" PRIx64
					", LID %u, Index %s - %u groups\n"
					"       GUID 0x%016" PRIx64
					", LID %u, Index %s - %u groups\n",
					sw_get_guid_ho
					(reference_sw_arr[p_sw->rank]),
					reference_sw_arr[p_sw->rank]->base_lid,
					tuple_to_str
					(reference_sw_arr[p_sw->rank]->tuple),
					reference_sw_arr[p_sw->
							 rank]->
					up_port_groups_num,
					sw_get_guid_ho(p_sw), p_sw->base_lid,
					tuple_to_str(p_sw->tuple),
					p_sw->up_port_groups_num);
				res = FALSE;
				break;
			}

			if (p_sw->rank != (tree_rank - 1) &&
			    reference_sw_arr[p_sw->
					     rank]->down_port_groups_num !=
			    p_sw->down_port_groups_num) {
				/* we're allowing some hca's to be missing */
				OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_ERROR,
					"ERR AB0A: Different number of downward port groups on switches:\n"
					"       GUID 0x%016" PRIx64
					", LID %u, Index %s - %u port groups\n"
					"       GUID 0x%016" PRIx64
					", LID %u, Index %s - %u port groups\n",
					sw_get_guid_ho
					(reference_sw_arr[p_sw->rank]),
					reference_sw_arr[p_sw->rank]->base_lid,
					tuple_to_str
					(reference_sw_arr[p_sw->rank]->tuple),
					reference_sw_arr[p_sw->
							 rank]->
					down_port_groups_num,
					sw_get_guid_ho(p_sw), p_sw->base_lid,
					tuple_to_str(p_sw->tuple),
					p_sw->down_port_groups_num);
				res = FALSE;
				break;
			}

			if (reference_sw_arr[p_sw->rank]->up_port_groups_num !=
			    0) {
				p_ref_group =
				    reference_sw_arr[p_sw->
						     rank]->up_port_groups[0];
				for (i = 0; i < p_sw->up_port_groups_num; i++) {
					p_group = p_sw->up_port_groups[i];
					if (cl_ptr_vector_get_size
					    (&p_ref_group->ports) !=
					    cl_ptr_vector_get_size
					    (&p_group->ports)) {
						OSM_LOG(&p_ftree->p_osm->log,
							OSM_LOG_ERROR,
							"ERR AB0B: Different number of ports in an upward port group on switches:\n"
							"       GUID 0x%016"
							PRIx64
							", LID %u, Index %s - %u ports\n"
							"       GUID 0x%016"
							PRIx64
							", LID %u, Index %s - %u ports\n",
							sw_get_guid_ho
							(reference_sw_arr
							 [p_sw->rank]),
							reference_sw_arr[p_sw->
									 rank]->
							base_lid,
							tuple_to_str
							(reference_sw_arr
							 [p_sw->rank]->tuple),
							cl_ptr_vector_get_size
							(&p_ref_group->ports),
							sw_get_guid_ho(p_sw),
							p_sw->base_lid,
							tuple_to_str(p_sw->
								     tuple),
							cl_ptr_vector_get_size
							(&p_group->ports));
						res = FALSE;
						break;
					}
				}
			}
			if (reference_sw_arr[p_sw->rank]->down_port_groups_num
			    != 0 && p_sw->rank != (tree_rank - 1)) {
				/* we're allowing some hca's to be missing */
				p_ref_group =
				    reference_sw_arr[p_sw->
						     rank]->down_port_groups[0];
				for (i = 0; i < p_sw->down_port_groups_num; i++) {
					p_group = p_sw->down_port_groups[0];
					if (cl_ptr_vector_get_size
					    (&p_ref_group->ports) !=
					    cl_ptr_vector_get_size
					    (&p_group->ports)) {
						OSM_LOG(&p_ftree->p_osm->log,
							OSM_LOG_ERROR,
							"ERR AB0C: Different number of ports in an downward port group on switches:\n"
							"       GUID 0x%016"
							PRIx64
							", LID %u, Index %s - %u ports\n"
							"       GUID 0x%016"
							PRIx64
							", LID %u, Index %s - %u ports\n",
							sw_get_guid_ho
							(reference_sw_arr
							 [p_sw->rank]),
							reference_sw_arr[p_sw->
									 rank]->
							base_lid,
							tuple_to_str
							(reference_sw_arr
							 [p_sw->rank]->tuple),
							cl_ptr_vector_get_size
							(&p_ref_group->ports),
							sw_get_guid_ho(p_sw),
							p_sw->base_lid,
							tuple_to_str(p_sw->
								     tuple),
							cl_ptr_vector_get_size
							(&p_group->ports));
						res = FALSE;
						break;
					}
				}
			}
		}		/* end of else */
	}			/* end of while */

	if (res == TRUE)
		OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_VERBOSE,
			"Fabric topology has been identified as FatTree\n");
	else
		OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_ERROR,
			"ERR AB0D: Fabric topology hasn't been identified as FatTree\n");

	free(reference_sw_arr);
	OSM_LOG_EXIT(&p_ftree->p_osm->log);
	return res;
}				/* fabric_validate_topology() */