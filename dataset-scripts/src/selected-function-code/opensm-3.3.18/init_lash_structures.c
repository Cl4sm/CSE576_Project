static int init_lash_structures(lash_t * p_lash)
{
	unsigned vl_min = p_lash->vl_min;
	unsigned num_switches = p_lash->num_switches;
	osm_log_t *p_log = &p_lash->p_osm->log;
	int status = 0;
	unsigned int i, j, k;

	OSM_LOG_ENTER(p_log);

	/* initialise cdg_vertex_matrix[num_switches][num_switches][num_switches] */
	p_lash->cdg_vertex_matrix =
	    (cdg_vertex_t ****) malloc(vl_min * sizeof(cdg_vertex_t ***));
	if (p_lash->cdg_vertex_matrix == NULL)
		goto Exit_Mem_Error;
	for (i = 0; i < vl_min; i++) {
		p_lash->cdg_vertex_matrix[i] =
		    (cdg_vertex_t ***) malloc(num_switches *
					      sizeof(cdg_vertex_t **));

		if (p_lash->cdg_vertex_matrix[i] == NULL)
			goto Exit_Mem_Error;
	}

	for (i = 0; i < vl_min; i++) {
		for (j = 0; j < num_switches; j++) {
			p_lash->cdg_vertex_matrix[i][j] =
			    (cdg_vertex_t **) malloc(num_switches *
						     sizeof(cdg_vertex_t *));
			if (p_lash->cdg_vertex_matrix[i][j] == NULL)
				goto Exit_Mem_Error;

			for (k = 0; k < num_switches; k++)
				p_lash->cdg_vertex_matrix[i][j][k] = NULL;
		}
	}

	/*
	 * initialise virtual_location[num_switches][num_switches][num_layers],
	 * default value = 0
	 */
	p_lash->virtual_location =
	    (int ***)malloc(num_switches * sizeof(int ***));
	if (p_lash->virtual_location == NULL)
		goto Exit_Mem_Error;

	for (i = 0; i < num_switches; i++) {
		p_lash->virtual_location[i] =
		    (int **)malloc(num_switches * sizeof(int **));
		if (p_lash->virtual_location[i] == NULL)
			goto Exit_Mem_Error;
	}

	for (i = 0; i < num_switches; i++) {
		for (j = 0; j < num_switches; j++) {
			p_lash->virtual_location[i][j] =
			    (int *)malloc(vl_min * sizeof(int *));
			if (p_lash->virtual_location[i][j] == NULL)
				goto Exit_Mem_Error;
			for (k = 0; k < vl_min; k++)
				p_lash->virtual_location[i][j][k] = 0;
		}
	}

	/* initialise num_mst_in_lane[num_switches], default 0 */
	memset(p_lash->num_mst_in_lane, 0,
	       IB_MAX_NUM_VLS * sizeof(p_lash->num_mst_in_lane[0]));

	goto Exit;

Exit_Mem_Error:
	status = -1;
	OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 4D01: "
		"Could not allocate required memory for LASH errno %d, errno %d for lack of memory\n",
		errno, ENOMEM);

Exit:
	OSM_LOG_EXIT(p_log);
	return status;
}